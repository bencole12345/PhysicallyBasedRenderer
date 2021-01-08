#include <string>
#include <PBR/PBR.h>

using namespace PBR;
using namespace PBR::physically_based;

void loadScene(std::shared_ptr<PhysicallyBasedScene>* scene)
{
    // Generate 16 spheres
    std::vector<std::shared_ptr<PhysicallyBasedSceneObject>> sceneObjects;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::string_view objPath = "example/resources/models/sphere-highpoly.obj";
            float x = -3.5f + (float) i * 7.0f / 3.0f;
            float y = -3.5f + (float) j * 7.0f / 3.0f;
            glm::vec3 position(x, y, 0.0f);
            glm::vec3 orientation(0.0f);
            float scale = 1.0f;
            PhysicallyBasedMaterial material{
                    .albedo = glm::vec3(0.25f + (float) i * 0.7f / 3.0f, 0.6f - 0.1f * i, 0.9f - (float) j * 0.4f / 3.0f),// 0.5f),
                    .roughness = 0.1f + (float) j * 0.7f / 3.0f,
                    .metallic = 0.1f + (float) i * 0.8f / 3.0f,
                    .F0 = glm::vec3(0.04f)
            };
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
            PointLightSource{.pos = glm::vec3(-2.0f, 1.0f, 4.0f), .colour = glm::vec3(1.0f, 1.0f,
                                                                                      1.0f), .intensity = 25.0f},
            PointLightSource{.pos = glm::vec3(1.0f, 2.0f, 3.0f), .colour = glm::vec3(1.0f), .intensity = 4.0f},
            PointLightSource{.pos = glm::vec3(0.0f, 0.0f, 2.0f), .colour = glm::vec3(1.0f), .intensity = 4.0f},
            PointLightSource{.pos = glm::vec3(4.0f, 5.0f, 4.0f), .colour = glm::vec3(1.0f), .intensity = 16.0f}
    };

    // Skybox
    std::vector<std::string_view> skyboxTextures{
            "example/resources/skyboxes/ocean_with_mountains/right.jpg",
            "example/resources/skyboxes/ocean_with_mountains/left.jpg",
            "example/resources/skyboxes/ocean_with_mountains/top.jpg",
            "example/resources/skyboxes/ocean_with_mountains/bottom.jpg",
            "example/resources/skyboxes/ocean_with_mountains/front.jpg",
            "example/resources/skyboxes/ocean_with_mountains/back.jpg"
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
