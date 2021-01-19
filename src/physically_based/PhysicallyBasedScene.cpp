#include "physically_based/PhysicallyBasedScene.h"

#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/functional/hash.hpp>

#include "core/PointLightSource.h"
#include "core/ShaderProgram.h"
#include "core/TexturePrecomputation.h"
#include "physically_based/EnvironmentMap.h"
#include "physically_based/PBRUtil.h"
#include "physically_based/PhysicallyBasedSceneObject.h"

namespace PBR::physically_based {

namespace {

struct PhysicallyBasedMaterialHasher {
    size_t operator()(const PhysicallyBasedMaterial& material) const
    {
        size_t seed = 0;
        boost::hash_combine(seed, boost::hash_value(material.albedo.r));
        boost::hash_combine(seed, boost::hash_value(material.albedo.g));
        boost::hash_combine(seed, boost::hash_value(material.albedo.b));
        boost::hash_combine(seed, boost::hash_value(material.roughness));
        boost::hash_combine(seed, boost::hash_value(material.metallic));
        boost::hash_combine(seed, boost::hash_value(material.F0.r));
        boost::hash_combine(seed, boost::hash_value(material.F0.g));
        boost::hash_combine(seed, boost::hash_value(material.F0.b));
        return seed;
    }
};

} // anonymous namespace

PhysicallyBasedScene::PhysicallyBasedScene(std::vector<std::shared_ptr<PhysicallyBasedSceneObject>> sceneObjects,
                                           std::vector<PointLightSource> lights,
                                           std::shared_ptr<EnvironmentMap> environmentMap)
        :Scene<PhysicallyBasedSceneObject>(std::move(sceneObjects), std::move(lights), glm::vec3(0.0f)),
         environmentMap(environmentMap),
         prefilteredEnvironmentMaps(),
         brdfIntegrationMaps()
{
    precomputePrefilteredEnvironmentMaps();
    precomputeBRDFIntegrationMaps();
}

std::shared_ptr<EnvironmentMap> PhysicallyBasedScene::getEnvironmentMap() const
{
    return environmentMap;
}

const std::vector<std::shared_ptr<Texture>>& PhysicallyBasedScene::getPrefilteredEnvironmentMaps() const
{
    return prefilteredEnvironmentMaps;
}

const std::vector<std::shared_ptr<Texture>>& PhysicallyBasedScene::getBRDFIntegrationMaps() const
{
    return brdfIntegrationMaps;
}

void PhysicallyBasedScene::precomputePrefilteredEnvironmentMaps()
{
    std::unordered_map<PhysicallyBasedMaterial, std::shared_ptr<Texture>, PhysicallyBasedMaterialHasher> prefilteredCache;
    prefilteredEnvironmentMaps.clear();
    for (const auto& object : getSceneObjectsList()) {
        auto it = prefilteredCache.find(object->material);
        if (it == prefilteredCache.end()) {
            // Not found, need to compute it and add it to the cache
            std::shared_ptr<Texture> prefilteredEnvironmentMap = computePrefilteredEnvironmentMap(object->material);
            prefilteredEnvironmentMaps.push_back(prefilteredEnvironmentMap);
            prefilteredCache.insert(std::make_pair(object->material, prefilteredEnvironmentMap));
        }
        else {
            // Found, use the one we computed already
            prefilteredEnvironmentMaps.push_back(it->second);
        }
    }
}

std::shared_ptr<Texture> PhysicallyBasedScene::computePrefilteredEnvironmentMap(const PhysicallyBasedMaterial& material)
{
    // Load the shader program
    auto vertexShaderPath = PBRUtil::pbrShadersDir() / "PrepVerticesForRenderingTexture.vert";
    auto fragmentShaderPath = PBRUtil::pbrShadersDir() / "ComputePreFilteredEnvironmentMap.frag";
    ShaderProgram shader(vertexShaderPath, fragmentShaderPath);

    // Code to set up uniforms
    constexpr unsigned int mipmapLevels = 5;
    auto setUniforms = [this, &shader, mipmapLevels, material](auto mipmapLevel) {
        float roughness = (float) mipmapLevel / (float) (mipmapLevels - 1);
        shader.resetUniforms();
        shader.setUniform("radianceMap", this->environmentMap->getRadianceMap());
        shader.setUniform("roughness", roughness);
        shader.setUniform("dCoefficients.k_TrowbridgeReitzGGX", material.brdfCoefficients.normalDistribution.k_TrowbridgeReitzGGX);
        shader.setUniform("dCoefficients.k_Beckmann", material.brdfCoefficients.normalDistribution.k_Beckman);
        shader.setUniform("gCoefficients.k_SchlickGGX", material.brdfCoefficients.geometricAttenutation.k_SchlickGGX);
        shader.setUniform("gCoefficients.k_CookTorrance", material.brdfCoefficients.geometricAttenutation.k_CookTorrance);
    };

    // Allocate a texture ready for rendering
    std::shared_ptr<Texture> texture(new Texture());

    // Render the texture
    constexpr unsigned int maxWidth = 512;
    constexpr unsigned int maxHeight = 512;
    TexturePrecomputation::renderToMipmappedTexture(texture, shader, maxWidth, maxHeight, mipmapLevels, setUniforms);

    return texture;
}

void PhysicallyBasedScene::precomputeBRDFIntegrationMaps()
{
    std::unordered_map<PhysicallyBasedMaterial, std::shared_ptr<Texture>, PhysicallyBasedMaterialHasher> brdfCache;
    brdfIntegrationMaps.clear();
    for (const auto& object : getSceneObjectsList()) {
        auto it = brdfCache.find(object->material);
        if (it == brdfCache.end()) {
            // Not found, need to compute it and add it to the cache
            std::shared_ptr<Texture> brdfIntegrationMap = computeBRDFIntegrationMap(object->material);
            brdfIntegrationMaps.push_back(brdfIntegrationMap);
            brdfCache.insert(std::make_pair(object->material, brdfIntegrationMap));
        }
        else {
            // Found, use the one we computed already
            brdfIntegrationMaps.push_back(it->second);
        }
    }
}

std::shared_ptr<Texture> PhysicallyBasedScene::computeBRDFIntegrationMap(const PhysicallyBasedMaterial& material)
{
    auto vertexShader = PBRUtil::pbrShadersDir() / "PrepVerticesForRenderingTexture.vert";
    auto fragmentShader = PBRUtil::pbrShadersDir() / "ComputeBRDFIntegrationMap.frag";
    ShaderProgram shaderProgram(vertexShader, fragmentShader);

    // Function for setting up shader uniforms
    auto prepareShaderUniforms = [material, &shaderProgram]() {
        shaderProgram.resetUniforms();
        shaderProgram.setUniform("dCoefficients.k_TrowbridgeReitzGGX", material.brdfCoefficients.normalDistribution.k_TrowbridgeReitzGGX);
        shaderProgram.setUniform("dCoefficients.k_Beckmann", material.brdfCoefficients.normalDistribution.k_Beckman);
        shaderProgram.setUniform("gCoefficients.k_SchlickGGX", material.brdfCoefficients.geometricAttenutation.k_SchlickGGX);
        shaderProgram.setUniform("gCoefficients.k_CookTorrance", material.brdfCoefficients.geometricAttenutation.k_CookTorrance);
    };

    std::shared_ptr<Texture> texture(new Texture());
    unsigned int width = 512, height = 512;
    TexturePrecomputation::renderToTexture(texture, shaderProgram, width, height, prepareShaderUniforms);
    return texture;
}

} // namespace PBR::physically_based
