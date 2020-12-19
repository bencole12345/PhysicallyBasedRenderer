#include "BasicPhongScene.h"

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

namespace PBR {

BasicPhongScene::BasicPhongScene(
        glm::vec3 backgroundColour,
        glm::vec3 ambientLight)
        :sceneObjects(),
         lightPositions(),
         lightColours(),
         backgroundColour(backgroundColour),
         ambientLight(ambientLight)
{
}

void BasicPhongScene::render(PBR::Camera& camera, double time)
{
    for (const auto& object : sceneObjects) {
        object->render(camera, time, ambientLight, lightPositions, lightColours);
    }
}

void BasicPhongScene::activate()
{
    glClearColor(backgroundColour.r, backgroundColour.g, backgroundColour.b, 1.0f);
}

void BasicPhongScene::addSceneObject(const std::shared_ptr<PhongSceneObject>& object)
{
    sceneObjects.insert(object);
}

void BasicPhongScene::addLightSource(const std::shared_ptr<LightSource>& lightSource)
{
    lightPositions.push_back(lightSource->pos);
    lightColours.push_back(lightSource->colour);
}

} // namespace PBR

