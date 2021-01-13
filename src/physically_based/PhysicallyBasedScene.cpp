#include "physically_based/PhysicallyBasedScene.h"

#include <memory>
#include <utility>
#include <vector>

#include "core/PointLightSource.h"
#include "physically_based/EnvironmentMap.h"
#include "physically_based/PhysicallyBasedSceneObject.h"

namespace PBR::physically_based {

PhysicallyBasedScene::PhysicallyBasedScene(std::vector<std::shared_ptr<PhysicallyBasedSceneObject>> sceneObjects,
                                           std::vector<PointLightSource> lights,
                                           std::shared_ptr<EnvironmentMap> environmentMap)
        :Scene<PhysicallyBasedSceneObject>(std::move(sceneObjects), std::move(lights), glm::vec3(0.0f)),
         environmentMap(std::move(environmentMap))
{
}

std::shared_ptr<EnvironmentMap> PhysicallyBasedScene::getEnvironmentMap() const
{
    return environmentMap;
}

} // namespace PBR::physically_based
