#include <filesystem>
#include <string>
#include <PBR/PBR.h>

using namespace PBR;
using namespace PBR::phong;

namespace fs = std::filesystem;

std::shared_ptr<PhongScene> loadScene()
{
    // Create the untextured orange cube
    float kD = 0.6f, kS = 0.4f, specularN = 2.0f;
    glm::vec3 colour(1.0f, 0.5f, 0.2f);
    PhongMaterial material{kD, kS, specularN, colour};
    glm::vec3 position(2.0f, 0.0f, 0.0f);
    glm::vec3 orientation(0.0f);
    float scale = 1.0f;
    std::shared_ptr<PhongSceneObject> colouredCube(
            new scene_objects::Cube(position,
                                    orientation,
                                    scale,
                                    material));

    // Create the textured cube
    kD = 0.8f;
    kS = 0.2f;
    specularN = 1.0f;
    material = {kD, kS, specularN};
    position = glm::vec3(-2.0f, 0.0f, 0.0f);
    orientation = glm::vec3(0.5f, 0.5f, 0.5f);
    scale = 2.0f;
    fs::path texturePath = fs::current_path() / "example" / "resources" / "textures" / "cookie.jpg";
    std::shared_ptr<Texture> texture(new Texture(texturePath));
    std::shared_ptr<PhongSceneObject> texturedCube(
            new scene_objects::Cube(position,
                                    orientation,
                                    scale,
                                    material,
                                    texture));

    // Create the plane
    colour = glm::vec3(0.3f);
    material = {kD, kS, specularN, colour};
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
    fs::path oceanWithMountains = fs::current_path() / "example" / "resources" / "skyboxes" / "ocean_with_mountains";
    std::vector<fs::path> skyboxTextures{
            oceanWithMountains / "right.jpg",
            oceanWithMountains / "left.jpg",
            oceanWithMountains / "top.jpg",
            oceanWithMountains / "bottom.jpg",
            oceanWithMountains / "front.jpg",
            oceanWithMountains / "back.jpg"
    };
    std::shared_ptr<phong::Skybox> skybox(new phong::Skybox(skyboxTextures));

    // Create the scene
    return std::make_shared<PhongScene>(sceneObjects,
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
    std::shared_ptr<PhongScene> scene = loadScene();

    // Create a Phong renderer
    std::shared_ptr<PhongRenderer> renderer(new PhongRenderer());

    // Run the main loop
    window.loopUntilClosed<PhongScene>(renderer, scene);

    return 0;
}
