#include "phong/PhongScene.h"

#include <utility>

#include <glm/vec3.hpp>

namespace PBR::phong {

PhongScene::PhongScene(std::vector<std::shared_ptr<PhongSceneObject>> sceneObjects, glm::vec3 ambientLight,
                       std::vector<PointLightSource> lights, glm::vec3 backgroundColour)
        :Scene(std::move(sceneObjects), std::move(lights), backgroundColour), ambientLight(ambientLight), skybox()
{
}

PhongScene::PhongScene(std::vector<std::shared_ptr<PhongSceneObject>> sceneObjects, glm::vec3 ambientLight,
                       std::vector<PointLightSource> lights, std::shared_ptr<skybox::Skybox> skybox)
        :Scene(std::move(sceneObjects), std::move(lights)), ambientLight(ambientLight), skybox(skybox)
{
}

glm::vec3 PhongScene::getAmbientLight() const
{
    return ambientLight;
}

bool PhongScene::hasSkybox() const
{
    return skybox.has_value();
}

const std::shared_ptr<skybox::Skybox>& PhongScene::getSkybox() const
{
    return skybox.value();
}

} // namespace PBR::phong
