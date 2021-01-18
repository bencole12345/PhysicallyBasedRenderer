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
    // Bunny obj file
    fs::path objPath = std::filesystem::current_path() / "example" / "resources" / "models" / "StanfordBunny.obj";

    // Copper bunny
    glm::vec3 position(0.0f, 0.0f, -2.0f);
    glm::vec3 orientation(0.0f, -1.2f, 0.0f);
    float scale = 0.6f;
    glm::vec3 albedo(0.63f ,0.13f ,0.17f);
    float roughness = 0.5f;
    float metallic = 1.0f;
    glm::vec3 F0 = FresnelValues::Copper;
    PhysicallyBasedMaterial material{albedo, roughness, metallic, F0};
    std::shared_ptr<PhysicallyBasedSceneObject> copperBunny(new scene_objects::CustomObject(objPath, position, orientation, material, scale));

    // Silver bunny
    position = glm::vec3(0.0f);
    material.roughness = 0.1f;
    material.metallic = 0.9f;
    material.F0 = FresnelValues::Silver;
    std::shared_ptr<PhysicallyBasedSceneObject> silverBunny(new scene_objects::CustomObject(objPath, position, orientation, material, scale));

    // Red plastic bunny
    position = glm::vec3(0.0f, 0.0f, 2.0f);
    material.roughness = 0.7;
    material.metallic = 0.0f;
    material.F0 = FresnelValues::PlasticLow;
    std::shared_ptr<PhysicallyBasedSceneObject> redPlasticBunny(new scene_objects::CustomObject(objPath, position, orientation, material, scale));

    // List of all objects
    std::vector<std::shared_ptr<PhysicallyBasedSceneObject>> sceneObjects{copperBunny, silverBunny, redPlasticBunny};

    // Point lights
    std::vector<PointLightSource> lights{
        PointLightSource{glm::vec3(-1.0f, 2.0f, 2.5f), glm::vec3(1.0f), 20.0f},
    };

    // Texture for the environment map
    auto environmentMapsDir = fs::current_path() / "example" / "resources" / "environment_maps";

    // Map 1: Utah desert
//    auto texturePath = environmentMapsDir / "Arches_E_PineTree" / "Arches_E_PineTree_3k.hdr";
//    auto sunDirection = PBRUtil::uvToCartesian(glm::vec2(0.583750f, 0.365000f));
//    DirectedLightSource sun{sunDirection, glm::vec3(254.0f/255.0f, 241.0f/255.0f, 224.0f/255.0f), 1.2f};
//    std::shared_ptr<EnvironmentMap> environmentMap(new EnvironmentMap(texturePath, sun));

    // Map 2: Malibu coast
//    auto texturePath = environmentMapsDir / "Malibu_Overlook" / "Malibu_Overlook_3k.hdr";
//    auto sunDirection = PBRUtil::uvToCartesian(glm::vec2(0.424f, 0.2f));
//    DirectedLightSource sun{sunDirection, glm::vec3(1.0f, 251.0f/255.0f, 232.0f/255.0f), 1.0f};
//    std::shared_ptr<EnvironmentMap> environmentMap(new EnvironmentMap(texturePath, sun));

    // Map 3: Winter forest
//    auto texturePath = environmentMapsDir / "Winter_Forest" / "WinterForest_Ref.hdr";
//    std::shared_ptr<EnvironmentMap> environmentMap(new EnvironmentMap(texturePath));

    // Map 4: Grand Canyon
    auto texturePath = environmentMapsDir / "GrandCanyon_C_YumaPoint" / "GCanyon_C_YumaPoint_3k.hdr";
    std::shared_ptr<EnvironmentMap> environmentMap(new EnvironmentMap(texturePath));

    // Create the scene
    return std::make_shared<PhysicallyBasedScene>(sceneObjects, lights, environmentMap);
}

int main()
{
    std::string title = "Physically Based Renderer: Bunnies";
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
