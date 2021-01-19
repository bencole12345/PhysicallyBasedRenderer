#include "physically_based/EnvironmentMap.h"

#include <filesystem>
#include <memory>
#include <optional>

#include "core/DirectedLightSource.h"
#include "core/ShaderProgram.h"
#include "core/Texture.h"
#include "core/TexturePrecomputation.h"
#include "physically_based/PBRUtil.h"

namespace fs = std::filesystem;

namespace PBR::physically_based {

/**
 * Precomputes the irradiance map of the background and returns it as a `Texture`.
 */
std::shared_ptr<Texture> precomputeIrradianceMap(std::shared_ptr<Texture> radianceMap)
{
    // Load the shader program we need to use
    auto precomputeIrradianceMapVertexShader =
            PBRUtil::pbrShadersDir() / "PrepVerticesForRenderingTexture.vert";
    auto precomputeIrradianceMapFragmentShader =
            PBRUtil::pbrShadersDir() / "ComputeIrradianceMap.frag";
    ShaderProgram shader(precomputeIrradianceMapVertexShader, precomputeIrradianceMapFragmentShader);

    // Code to set up uniforms
    auto prepareShaderUniforms = [&shader, radianceMap]() {
        shader.resetUniforms();
        shader.setUniform("radianceMap", radianceMap);
    };

    // Allocate a texture for rendering
    std::shared_ptr<Texture> texture(new Texture());

    // Render to the texture
    int width = 16;
    int height = 16;
    TexturePrecomputation::renderToTexture(texture, shader, width, height, prepareShaderUniforms);

    return texture;
}

EnvironmentMap::EnvironmentMap(const fs::path& texturePath,
                               std::optional<DirectedLightSource> sun)
        :radianceMap(new Texture(texturePath, true)),
         irradianceMap(precomputeIrradianceMap(radianceMap)),
         sun(sun)
{
}

std::shared_ptr<Texture> EnvironmentMap::getRadianceMap() const
{
    return radianceMap;
}

std::shared_ptr<Texture> EnvironmentMap::getIrradianceMap() const
{
    return irradianceMap;
}

const std::optional<DirectedLightSource>& EnvironmentMap::getSun() const
{
    return sun;
}

} // namespace PBR::physically_based
