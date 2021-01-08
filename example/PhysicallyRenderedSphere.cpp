#include <string>
#include <PBR/PBR.h>

using namespace PBR;
using namespace PBR::physically_based;

void loadScene(std::shared_ptr<PhysicallyBasedScene>* scene)
{
    // Load the sphere object
    std::string_view objPath = "example/resources/models/sphere-smoothnormals.obj";
    glm::vec3 position(-1.5f, 0.0f, 0.0f);
    glm::vec3 orientation(0.0f);
    float scale = 1.0f;
    PhysicallyBasedMaterial material{.albedo = glm::vec3(0.8f, 0.4f, 0.4f), .roughness = 0.5f, .metallic = 0.5f};
    std::shared_ptr<PhysicallyBasedSceneObject> sphere(
            new scene_objects::CustomObject(objPath, position, orientation, material, scale));

    objPath = "example/resources/models/SmoothSuzanne.obj";
    position = glm::vec3(1.5f, 0.0f, 0.0f);
    material.albedo = glm::vec3(0.4f, 0.8f, 0.4f);
    std::shared_ptr<PhysicallyBasedSceneObject> monkey(
            new scene_objects::CustomObject(objPath, position, orientation, material, scale));

    // All objects
    std::vector<std::shared_ptr<PhysicallyBasedSceneObject>> sceneObjects{
            sphere,
            monkey,
    };

    // Scene colour information
    glm::vec3 backgroundColour(37.0f / 255.0f, 66.0f / 255.0f, 79.0f / 255.0f);
    glm::vec3 ambientLight(0.5f, 0.5f, 0.5f);

    // Point lights
    std::vector<PointLightSource> lights{
            PointLightSource(glm::vec3(-1.5f, 3.0f, -1.0f), glm::vec3(1.0f, 1.0f, 0.8f)),
            PointLightSource(glm::vec3(1.0f, 4.0f, -1.0f), glm::vec3(0.5f)),
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
    *scene = std::make_shared<PhysicallyBasedScene>(sceneObjects, ambientLight, lights, skybox);
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
