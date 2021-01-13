#include <filesystem>
#include <string>
#include <PBR/PBR.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

using namespace PBR;
using namespace PBR::physically_based;

namespace fs = std::filesystem;

void loadScene(std::shared_ptr<PhysicallyBasedScene>* scene)
{
    // Generate 16 spheres
    fs::path objPath = std::filesystem::current_path() / "example" / "resources" / "models" / "sphere-highpoly.obj";

    std::vector<std::shared_ptr<PhysicallyBasedSceneObject>> sceneObjects;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            float x = -3.5f + (float) i * 7.0f / 3.0f;
            float y = -3.5f + (float) j * 7.0f / 3.0f;
            glm::vec3 position(x, y, 0.0f);
            glm::vec3 orientation(0.0f);
            float scale = 1.0f;
            glm::vec3 albedo(0.25f + (float) i * 0.7f / 3.0f, 0.6f - 0.1f * i, 0.9f - (float) j * 0.4f / 3.0f);
            float roughness = 0.1f + (float) j * 0.7f / 3.0f;
            float metallic = 0.1f + (float) i * 0.7f / 3.0f;
            glm::vec3 F0 = FresnelValues::PlasticLow;
            PhysicallyBasedMaterial material{albedo, roughness, metallic, F0};
            std::shared_ptr<PhysicallyBasedSceneObject> object(
                    new scene_objects::CustomObject(objPath, position, orientation, material, scale));
            sceneObjects.push_back(std::move(object));
        }
    }

    // Point lights
    std::vector<PointLightSource> lights{
//            PointLightSource{glm::vec3(-2.0f, 1.0f, 4.0f), glm::vec3(1.0f, 1.0f, 1.0f), 25.0f},
//            PointLightSource{glm::vec3(1.0f, 2.0f, 3.0f), glm::vec3(1.0f), 4.0f},
//            PointLightSource{glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(1.0f), 4.0f},
//            PointLightSource{glm::vec3(4.0f, 5.0f, 4.0f), glm::vec3(1.0f), 16.0f}
    };

    auto environmentMapDir = fs::current_path() / "example" / "resources" / "environment_maps" / "Arches_E_PineTree";
    auto skyboxTexturePath = environmentMapDir / "Arches_E_PineTree_8k.jpg";
    auto diffuseIrradianceMapPath = environmentMapDir / "Arches_E_PineTree_Env.hdr";
    auto specularIrradianceMapPath = environmentMapDir / "Arches_E_PineTree_3k.hdr";
    auto sunDirection = Util::uvToCartesian(glm::vec2(0.583750f, 0.365000f));
    DirectedLightSource sun{sunDirection, glm::vec3(254.0f/255.0f, 241.0f/255.0f, 224.0f/255.0f), 1.2f};
    std::shared_ptr<EnvironmentMap> environmentMap(
            new EnvironmentMap(skyboxTexturePath, diffuseIrradianceMapPath, specularIrradianceMapPath, sun));

    // Create the scene
    *scene = std::make_shared<PhysicallyBasedScene>(sceneObjects, lights, environmentMap);
}

int main()
{
    std::string title = "Physically Based Renderer: Physically Rendered Sphere Example";
    int width = 800;
    int height = 600;

    // Create a window
    Window window(title, width, height);

    // Create a scene
    std::shared_ptr<PhysicallyBasedScene> scene;
    loadScene(&scene);

    // Create a renderer
    std::shared_ptr<PhysicallyBasedRenderer> renderer(new physically_based::PhysicallyBasedRenderer());

    // Run the main loop
    window.loopUntilClosed<PhysicallyBasedScene>(renderer, scene);

    return 0;
}
