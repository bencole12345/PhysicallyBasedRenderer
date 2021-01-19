#include <filesystem>
#include <string>
#include <PBR/PBR.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

using namespace PBR;
using namespace PBR::physically_based;

namespace fs = std::filesystem;

std::shared_ptr<PhysicallyBasedScene> loadScene()
{
    // Generate the spheres and lights
    fs::path objPath = std::filesystem::current_path() / "example" / "resources" / "models" / "SphereHighPoly.obj";
    std::vector<std::shared_ptr<PhysicallyBasedSceneObject>> sceneObjects;
    std::vector<PointLightSource> lights;
    for (int i = 0; i < 5; i++) {
        glm::vec3 position(2.1f * i, 0.0f, 0.0f);
        glm::vec3 orientation(0.0f);
        float scale = 1.0f;
        glm::vec3 albedo(0.8f);
        float roughness = 0.5f;
        float metallic = 0.8f;
        glm::vec3 F0 = FresnelValues::Silver;
        PhysicallyBasedMaterial material{albedo, roughness, metallic, F0};
        material.brdfCoefficients.normalDistribution = NormalDistributionFunctionCoefficients{(float) (4 - i) / 4, (float) i / 4};
        material.brdfCoefficients.geometricAttenutation = GeometricAttenuationFunctionCoefficients{(float) (4 - i) / 4, (float) i / 4};
        std::shared_ptr<PhysicallyBasedSceneObject> object(
                new scene_objects::CustomObject(objPath, position, orientation, material, scale));
        sceneObjects.push_back(object);

        // Put a light in front of each sphere
        lights.push_back(PointLightSource{position + glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(1.0f), 20.0f});
    }

    // Texture for the environment map
    auto environmentMapsDir = fs::current_path() / "example" / "resources" / "environment_maps";

    // Map 1: Utah desert
    auto texturePath = environmentMapsDir / "Arches_E_PineTree" / "Arches_E_PineTree_3k.hdr";
    auto sunDirection = PBRUtil::uvToCartesian(glm::vec2(0.583750f, 0.365000f));
    DirectedLightSource sun{sunDirection, glm::vec3(254.0f / 255.0f, 241.0f / 255.0f, 224.0f / 255.0f), 1.2f};
    std::shared_ptr<EnvironmentMap> environmentMap(new EnvironmentMap(texturePath, sun));

    // Map 2: Malibu coast
//    auto texturePath = environmentMapsDir / "Malibu_Overlook" / "Malibu_Overlook_3k.hdr";
//    auto sunDirection = PBRUtil::uvToCartesian(glm::vec2(0.424f, 0.2f));
//    DirectedLightSource sun{sunDirection, glm::vec3(1.0f, 251.0f/255.0f, 232.0f/255.0f), 1.0f};
//    std::shared_ptr<EnvironmentMap> environmentMap(new EnvironmentMap(texturePath, sun));

    // Map 3: Winter forest
//    auto texturePath = environmentMapsDir / "Winter_Forest" / "WinterForest_Ref.hdr";
//    std::shared_ptr<EnvironmentMap> environmentMap(new EnvironmentMap(texturePath));

    // Create the scene
    return std::make_shared<PhysicallyBasedScene>(sceneObjects, lights, environmentMap);
}

int main()
{
    std::string title = "Physically Based Renderer: Physically Rendered Spheres";
    int width = 1024;
    int height = 768;

    // Create a window
    Window window(title, width, height);

    // Create a scene
    std::shared_ptr<PhysicallyBasedScene> scene = loadScene();

    // Create a renderer
    std::shared_ptr<PhysicallyBasedRenderer> renderer(new physically_based::PhysicallyBasedRenderer());

    // Run the main loop
    window.loopUntilClosed<PhysicallyBasedScene>(renderer, scene);

    return 0;
}
