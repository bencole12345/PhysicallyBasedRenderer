#include <string>
#include <PBR/PBR.h>

using namespace PBR;
using namespace PBR::phong;

void loadScene(std::shared_ptr<PhongScene>* scene)
{
    // Create the untextured orange cube
    PhongMaterial material{.kD = 0.6f, .kS = 0.4f, .specularN = 2.0f, .colour = glm::vec3(1.0f, 0.5f, 0.2f)};
    glm::vec3 position(2.0f, 0.0f, 0.0f);
    glm::vec3 orientation(0.0f);
    float scale = 1.0f;
    std::shared_ptr<PhongSceneObject> colouredCube(
            new scene_objects::Cube(position,
                                    orientation,
                                    scale,
                                    material));

    // Create the textured cube
    material = {.kD = 0.8f, .kS = 0.2f, .specularN = 1.0f};
    position = glm::vec3(-2.0f, 0.0f, 0.0f);
    orientation = glm::vec3(0.5f, 0.5f, 0.5f);
    scale = 2.0f;
    std::shared_ptr<Texture> texture(
            new Texture("example/resources/textures/cookie.jpg"));
    std::shared_ptr<PhongSceneObject> texturedCube(
            new scene_objects::Cube(position,
                                    orientation,
                                    scale,
                                    material,
                                    texture));

    // Create the plane
    material = {.kD = 0.8f, .kS = 0.2f, .specularN = 1.0f, .colour = glm::vec3(0.3f, 0.3f, 0.3f)};
    position = glm::vec3(0.0f, -2.0f, 0.0f);
    orientation = glm::vec3(0.0f);
    glm::vec2 dimensions(20.0f, 20.0f);
    std::shared_ptr<PhongSceneObject> plane(
            new scene_objects::Plane(position,
                                     orientation,
                                     dimensions,
                                     material));

    // All objects
    std::vector<std::shared_ptr<PhongSceneObject>> sceneObjects{
            colouredCube,
            texturedCube,

            // Uncomment if you want to include the plane
//            plane
    };

    // Scene colour information
    glm::vec3 backgroundColour(0.0f, 0.0f, 0.0f);
    glm::vec3 ambientLight(0.5f, 0.5f, 0.5f);

    // Point lights
    std::vector<PointLightSource> lights{
            PointLightSource{glm::vec3(2.0f, -1.0f, 2.0f), glm::vec3(1.0f)},
            PointLightSource{glm::vec3(-1.0f, 5.0f, -1.0f), glm::vec3(1.0f)}
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
    std::shared_ptr<skybox::Skybox> skybox(new skybox::Skybox(skyboxTextures));

    // Create the scene
    *scene = std::make_shared<PhongScene>(sceneObjects,
                                          ambientLight,
                                          lights,
                                          skybox);
}

int main()
{
    std::string title = "Physically Based Renderer: Cubes with Skybox Example [Phong]";
    int width = 800;
    int height = 600;

    // Create a window
    Window window(title, width, height);

    // Create a scene
    std::shared_ptr<PhongScene> scene;
    loadScene(&scene);

    // Create a Phong renderer
    std::shared_ptr<PhongRenderer> renderer(new PhongRenderer());

    // Run the main loop
    window.loopUntilClosed<PhongScene>(renderer, scene);

    return 0;
}
