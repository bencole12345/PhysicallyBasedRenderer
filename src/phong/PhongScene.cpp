#include "phong/PhongScene.h"

#include <memory>
#include <set>

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

#include "core/PointLightSource.h"
#include "core/Skybox.h"
#include "phong/PhongSceneObject.h"

namespace PBR {
namespace phong {

PhongScene::PhongScene(
        glm::vec3 backgroundColour,
        glm::vec3 ambientLight)
        :sceneObjects(),
         lightPositions(),
         lightColours(),
         backgroundColour(backgroundColour),
         ambientLight(ambientLight)
{
}

void PhongScene::render(const PBR::Camera& camera, double time)
{
    // Render all scene objects
    for (const auto& object : sceneObjects) {
        object->render(camera, time, ambientLight, lightPositions, lightColours);
    }

    // Render the skybox to remaining fragments
    if (skybox.has_value()) {
        skybox->get()->draw(camera);
    }
}

void PhongScene::activate()
{
    glClearColor(backgroundColour.r, backgroundColour.g, backgroundColour.b, 1.0f);
}

void PhongScene::addSceneObject(const std::shared_ptr<PhongSceneObject>& object)
{
    sceneObjects.push_back(object);
}

void PhongScene::addLightSource(const std::shared_ptr<PointLightSource>& lightSource)
{
    lightPositions.push_back(lightSource->pos);
    lightColours.push_back(lightSource->colour);
}

void PhongScene::addSkybox(std::shared_ptr<Skybox> skybox)
{
    this->skybox = skybox;
}

void PhongScene::removeSkybox()
{
    skybox.reset();
}

} // namespace phong
} // namespace PBR

