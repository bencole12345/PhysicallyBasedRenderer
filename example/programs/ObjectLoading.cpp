#include <filesystem>
#include <string>
#include <PBR/PBR.h>

using namespace PBR;
using namespace PBR::phong;

namespace fs = std::filesystem;

std::shared_ptr<PhongScene> loadScene()
{
    // Load the viking room object
    fs::path objPath = fs::current_path() / "example" / "resources" / "models" / "VikingRoom.obj";
    fs::path texturePath = fs::current_path() / "example" / "resources" / "textures" / "viking_room.png";
    std::shared_ptr<Texture> texture(new Texture(texturePath));
    glm::vec3 position(-2.0f, 0.0f, 0.0f);
    glm::vec3 orientation(0.0f);
    float scale = 10.0f;
    float kD = 0.8f;
    float kS = 0.2f;
    float specularN = 1.0f;
    PhongMaterial material{kD, kS, specularN};
    std::shared_ptr<PhongSceneObject> vikingRoom(
            new scene_objects::CustomObject(objPath, position, orientation, material, scale, texture));

    // Load the monkey
    objPath = fs::current_path() / "example" / "resources" / "models" / "SuzanneFlat.obj";
    position = glm::vec3(1.0f, 1.0f, 0.0f);
    orientation = glm::vec3(0.0f);
    scale = 0.8f;
    glm::vec3 colour(0.8f, 0.1f, 0.1f);
    material = {kD, kS, specularN, colour};
    std::shared_ptr<PhongSceneObject> monkey(
            new scene_objects::CustomObject(objPath, position, orientation, material, scale));

    // All objects
    std::vector<std::shared_ptr<PhongSceneObject>> sceneObjects{
            vikingRoom,
            monkey,
    };

    // Scene colour information
    glm::vec3 backgroundColour(37.0f / 255.0f, 66.0f / 255.0f, 79.0f / 255.0f);
    glm::vec3 ambientLight(0.5f, 0.5f, 0.5f);

    // Point lights
    std::vector<PointLightSource> lights{
            PointLightSource{glm::vec3(-1.5f, 3.0f, -1.0f), glm::vec3(1.0f, 1.0f, 0.8f)},
            PointLightSource{glm::vec3(1.0f, 4.0f, -1.0f), glm::vec3(0.5f)},
    };

    // Create the scene
    return std::make_shared<PhongScene>(sceneObjects, ambientLight, lights, backgroundColour);
}

int main()
{
    std::string title = "Physically Based Renderer: Object Loading Example [Phong]";
    int width = 800;
    int height = 600;

    // Create a window
    Window window(title, width, height);

    // Create a scene
    std::shared_ptr<PhongScene> scene = loadScene();

    // Create a Phong renderer
    std::shared_ptr<Renderer<PhongScene>> renderer(new PhongRenderer());

    // Run the main loop
    window.loopUntilClosed(renderer, scene);

    return 0;
}
