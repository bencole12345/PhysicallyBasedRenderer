#ifndef PHYSICALLYBASEDRENDERER_PHYSICALLYBASEDSCENE
#define PHYSICALLYBASEDRENDERER_PHYSICALLYBASEDSCENE

#include <memory>
#include <vector>

#include "core/PointLightSource.h"
#include "core/Scene.h"
#include "physically_based/EnvironmentMap.h"
#include "physically_based/PhysicallyBasedSceneObject.h"

namespace PBR::physically_based {

class PhysicallyBasedScene : public Scene<PhysicallyBasedSceneObject> {
private:
    std::shared_ptr<EnvironmentMap> environmentMap;

public:
    PhysicallyBasedScene(std::vector<std::shared_ptr<PhysicallyBasedSceneObject>> sceneObjects,
                         std::vector<PointLightSource> lights, std::shared_ptr<EnvironmentMap> environmentMap);

    std::shared_ptr<EnvironmentMap> getEnvironmentMap() const;
};

} // namespace PBR::physically_based

#endif //PHYSICALLYBASEDRENDERER_PHYSICALLYBASEDSCENE
