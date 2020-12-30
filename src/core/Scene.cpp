#include "core/Scene.h"

#include <memory>
#include <utility>
#include <vector>

#include <glm/vec3.hpp>

#include "core/PointLightSource.h"
#include "core/SceneObject.h"
#include "core/Skybox.h"

namespace PBR {

Scene::Scene(std::vector<std::shared_ptr<SceneObject>> sceneObjects, glm::vec3 ambientLight,
             std::vector<PointLightSource> lights, glm::vec3 backgroundColour)
        :sceneObjects(std::move(sceneObjects)),
         ambientLight(ambientLight),
         lights(std::move(lights)),
         lightPositions(),
         lightColours(),
         backgroundColour(backgroundColour),
         skybox({})
{
    for (const PointLightSource& light : this->lights) {
        lightPositions.push_back(light.pos);
        lightColours.push_back(light.colour);
    }
}

Scene::Scene(std::vector<std::shared_ptr<SceneObject>> sceneObjects, glm::vec3 ambientLight,
             std::vector<PointLightSource> lights, std::shared_ptr<Skybox> skybox)
        :sceneObjects(std::move(sceneObjects)),
         ambientLight(ambientLight),
         lights(std::move(lights)),
         lightPositions(),
         lightColours(),
         backgroundColour(0.0f),
         skybox(skybox)
{
    for (const PointLightSource& light : this->lights) {
        lightPositions.push_back(light.pos);
        lightColours.push_back(light.colour);
    }
}

const std::vector<std::shared_ptr<SceneObject>>& Scene::getSceneObjectsList() const
{
    return sceneObjects;
}

void Scene::addObject(const std::shared_ptr<SceneObject>& object)
{
    sceneObjects.push_back(object);
}

glm::vec3 Scene::getAmbientLight() const
{
    return ambientLight;
}

const std::vector<PointLightSource>& Scene::getLights() const
{
    return lights;
}

const std::vector<glm::vec3>& Scene::getLightPositions() const
{
    return lightPositions;
}

const std::vector<glm::vec3>& Scene::getLightColours() const
{
    return lightColours;
}

void Scene::addLight(const PointLightSource& light)
{
    lights.push_back(light);
    lightPositions.push_back(light.pos);
    lightColours.push_back(light.colour);
}

glm::vec3 Scene::getBackgroundColour() const
{
    return backgroundColour;
}

bool Scene::hasSkybox() const
{
    return skybox.has_value();
}

std::shared_ptr<Skybox> Scene::getSkybox() const
{
    assert(skybox.has_value());
    return skybox.value();
}

} // namespace PBR
