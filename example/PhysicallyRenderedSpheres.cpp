#include <filesystem>
#include <string>
#include <PBR/PBR.h>

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
            glm::vec3 F0(0.04f);
            PhysicallyBasedMaterial material{albedo, roughness, metallic, F0};
            std::shared_ptr<PhysicallyBasedSceneObject> object(
                    new scene_objects::CustomObject(objPath, position, orientation, material, scale));
            sceneObjects.push_back(std::move(object));
        }
    }

    // Scene colour information
    glm::vec3 backgroundColour(0.1f);
    glm::vec3 ambientLight(0.5f, 0.5f, 0.5f);

    // Point lights
    std::vector<PointLightSource> lights{
            PointLightSource{glm::vec3(-2.0f, 1.0f, 4.0f), glm::vec3(1.0f, 1.0f, 1.0f), 25.0f},
            PointLightSource{glm::vec3(1.0f, 2.0f, 3.0f), glm::vec3(1.0f), 4.0f},
            PointLightSource{glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(1.0f), 4.0f},
            PointLightSource{glm::vec3(4.0f, 5.0f, 4.0f), glm::vec3(1.0f), 16.0f}
    };

    // Skybox
    fs::path oceanWithMountains = fs::current_path() / "example" / "resources" / "skyboxes" / "ocean_with_mountains";
    std::vector<fs::path> skyboxTextures{
            oceanWithMountains / "right.jpg",
            oceanWithMountains / "left.jpg",
            oceanWithMountains / "top.jpg",
            oceanWithMountains / "bottom.jpg",
            oceanWithMountains / "front.jpg",
            oceanWithMountains / "back.jpg"
    };
    std::shared_ptr<skybox::Skybox> skybox(new skybox::Skybox(skyboxTextures));

    // Create the scene
    *scene = std::make_shared<PhysicallyBasedScene>(sceneObjects, ambientLight, lights, backgroundColour);
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
