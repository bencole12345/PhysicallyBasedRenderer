#include <iostream>
#include <string>

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

#include <PBR/PBR.h>

void loadScene(std::shared_ptr<PBR::Scene>* scene)
{
    // Load the viking room object
    std::string_view objPath = "example/resources/models/viking_room.obj";
    std::string_view texturePath = "example/resources/textures/viking_room.png";
    glm::vec3 position(-2.0f, 0.0f, 0.0f);
    glm::vec3 orientation(0.0f);
    float scale = 10.0f;
    PBR::Material material{.kD = 0.8f, .kS = 0.2f, .specularN = 1.0f};
    std::shared_ptr<PBR::SceneObject> vikingRoom(
            new PBR::scene_objects::CustomObject(objPath, texturePath, position, orientation, material, scale));

    // Load the monkey
    objPath = "example/resources/models/monkey.obj";
    position = glm::vec3(1.0f, 1.0f, 0.0f);
    orientation = glm::vec3(0.0f);
    scale = 0.8f;
    glm::vec3 colour(0.8f, 0.1f, 0.1f);
    std::shared_ptr<PBR::SceneObject> monkey(
            new PBR::scene_objects::CustomObject(objPath, colour, position, orientation, material, scale));

    // All objects
    std::vector<std::shared_ptr<PBR::SceneObject>> sceneObjects{
            vikingRoom,
            monkey,
    };

    // Scene colour information
    glm::vec3 backgroundColour(37.0f / 255.0f, 66.0f / 255.0f, 79.0f / 255.0f);
    glm::vec3 ambientLight(0.5f, 0.5f, 0.5f);

    // Point lights
    std::vector<PBR::PointLightSource> lights{
            PBR::PointLightSource(glm::vec3(-1.5f, 3.0f, -1.0f), glm::vec3(1.0f, 1.0f, 0.8f)),
            PBR::PointLightSource(glm::vec3(1.0f, 4.0f, -1.0f), glm::vec3(0.5f)),
    };

    // Create the scene
    *scene = std::make_shared<PBR::Scene>(sceneObjects, ambientLight, lights, backgroundColour);
}

int main()
{
    std::string title = "Physically Based Renderer: Object Loading Example";
    int width = 800;
    int height = 600;

    if (!glfwInit()) {
        std::cerr << "Failed to initialise GLFW." << std::endl;
        return 1;
    }

    // Create a window
    PBR::Window window(title, width, height);

    // Create a scene
    std::shared_ptr<PBR::Scene> scene;
    loadScene(&scene);

    // Create a Phong renderer
    std::shared_ptr<PBR::Renderer> renderer(new PBR::phong::PhongRenderer());

    // Run the main loop
    window.loopUntilClosed(renderer, scene);

    return 0;
}
