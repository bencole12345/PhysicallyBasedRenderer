#include "example/ExampleSceneBuilder.h"

#include <memory>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "core/Material.h"
#include "core/SceneObject.h"
#include "core/Texture.h"
#include "scene_objects/Cube.h"
#include "scene_objects/Plane.h"

namespace PBR {

void ExampleSceneBuilder::loadBasicScene(std::shared_ptr<Scene>* scene)
{
    // Create the untextured orange cube
    Material material{.kD = 0.6f, .kS = 0.4f, .specularN = 2.0f};
    glm::vec3 position(2.0f, 0.0f, 0.0f);
    glm::vec3 orientation(0.0f);
    float scale = 1.0f;
    glm::vec3 colour(1.0f, 0.5f, 0.2f);
    std::shared_ptr<SceneObject> colouredCube(
            new scene_objects::Cube(position,
                                    orientation,
                                    scale,
                                    material,
                                    colour));

    // Create the textured cube
    material = {.kD = 0.8f, .kS = 0.2f, .specularN = 1.0f};
    position = glm::vec3(-2.0f, 0.0f, 0.0f);
    orientation = glm::vec3(0.5f, 0.5f, 0.5f);
    scale = 2.0f;
    std::shared_ptr<Texture> texture(new Texture("resources/textures/cookie.jpg"));
    std::shared_ptr<SceneObject> texturedCube(
            new scene_objects::Cube(position,
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
    std::shared_ptr<SceneObject> plane(
            new scene_objects::Plane(position,
                                     orientation,
                                     dimensions,
                                     material,
                                     colour));

    // All objects
    std::vector<std::shared_ptr<SceneObject>> sceneObjects{
            colouredCube,
            texturedCube,
            plane
    };

    // Scene colour information
    glm::vec3 backgroundColour(0.0f, 0.0f, 0.0f);
    glm::vec3 ambientLight(0.5f, 0.5f, 0.5f);

    // Point lights
    std::vector<PointLightSource> lights{
            PointLightSource(glm::vec3(2.0f, -1.0f, 2.0f), glm::vec3(1.0f)),
            PointLightSource(glm::vec3(-1.0f, 5.0f, -1.0f), glm::vec3(1.0f))
    };

    // Set up the skybox
    std::vector<std::string_view> skyboxTextures{
            "resources/skyboxes/ocean_with_mountains/right.jpg",
            "resources/skyboxes/ocean_with_mountains/left.jpg",
            "resources/skyboxes/ocean_with_mountains/top.jpg",
            "resources/skyboxes/ocean_with_mountains/bottom.jpg",
            "resources/skyboxes/ocean_with_mountains/front.jpg",
            "resources/skyboxes/ocean_with_mountains/back.jpg"
    };
    std::shared_ptr<Skybox> skybox(new Skybox(skyboxTextures));

    // Create the scene
    *scene = std::make_shared<Scene>(sceneObjects,
                                     ambientLight,
                                     lights,
                                     skybox);
}

} // namespace PBR
