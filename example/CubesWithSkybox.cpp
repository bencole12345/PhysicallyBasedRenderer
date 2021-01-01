#include <iostream>
#include <string>

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

#include <PBR/PBR.h>

void loadScene(std::shared_ptr<PBR::Scene>* scene)
{
    // Create the untextured orange cube
    PBR::Material material{.kD = 0.6f, .kS = 0.4f, .specularN = 2.0f};
    glm::vec3 position(2.0f, 0.0f, 0.0f);
    glm::vec3 orientation(0.0f);
    float scale = 1.0f;
    glm::vec3 colour(1.0f, 0.5f, 0.2f);
    std::shared_ptr<PBR::SceneObject> colouredCube(
            new PBR::scene_objects::Cube(position,
                                    orientation,
                                    scale,
                                    material,
                                    colour));

    // Create the textured cube
    material = {.kD = 0.8f, .kS = 0.2f, .specularN = 1.0f};
    position = glm::vec3(-2.0f, 0.0f, 0.0f);
    orientation = glm::vec3(0.5f, 0.5f, 0.5f);
    scale = 2.0f;
    std::shared_ptr<PBR::Texture> texture(
            new PBR::Texture("example/resources/textures/cookie.jpg"));
    std::shared_ptr<PBR::SceneObject> texturedCube(
            new PBR::scene_objects::Cube(position,
                                    orientation,
                                    scale,
                                    material,
                                    texture));

    // Create the plane
    material = {.kD = 0.8f, .kS = 0.2f, .specularN = 1.0f};
    position = glm::vec3(0.0f, -2.0f, 0.0f);
    orientation = glm::vec3(0.0f);
    glm::vec2 dimensions(20.0f, 20.0f);
    colour = glm::vec3(0.3f, 0.3f, 0.3f);
    std::shared_ptr<PBR::SceneObject> plane(
            new PBR::scene_objects::Plane(position,
                                     orientation,
                                     dimensions,
                                     material,
                                     colour));

    // All objects
    std::vector<std::shared_ptr<PBR::SceneObject>> sceneObjects{
            colouredCube,
            texturedCube,

            // Uncomment if you want to include the plane
//            plane
    };

    // Scene colour information
    glm::vec3 backgroundColour(0.0f, 0.0f, 0.0f);
    glm::vec3 ambientLight(0.5f, 0.5f, 0.5f);

    // Point lights
    std::vector<PBR::PointLightSource> lights{
            PBR::PointLightSource(glm::vec3(2.0f, -1.0f, 2.0f), glm::vec3(1.0f)),
            PBR::PointLightSource(glm::vec3(-1.0f, 5.0f, -1.0f), glm::vec3(1.0f))
    };

    // Set up the skybox
    std::vector<std::string_view> skyboxTextures{
            "example/resources/skyboxes/ocean_with_mountains/right.jpg",
            "example/resources/skyboxes/ocean_with_mountains/left.jpg",
            "example/resources/skyboxes/ocean_with_mountains/top.jpg",
            "example/resources/skyboxes/ocean_with_mountains/bottom.jpg",
            "example/resources/skyboxes/ocean_with_mountains/front.jpg",
            "example/resources/skyboxes/ocean_with_mountains/back.jpg"
    };
    std::shared_ptr<PBR::Skybox> skybox(new PBR::Skybox(skyboxTextures));

    // Create the scene
    *scene = std::make_shared<PBR::Scene>(sceneObjects,
                                     ambientLight,
                                     lights,
                                     skybox);
}

int main()
{
    std::string title = "Physically Based Renderer: Cubes with Skybox Example";
    int width = 800;
    int height = 600;

    // TODO: Move this to the Window class
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
