#ifndef PHYSICALLYBASEDRENDERER_PHYSICALLYBASEDSCENE
#define PHYSICALLYBASEDRENDERER_PHYSICALLYBASEDSCENE

#include <iterator>
#include <memory>
#include <unordered_map>
#include <vector>

#include "core/PointLightSource.h"
#include "core/Scene.h"
#include "core/Texture.h"
#include "physically_based/EnvironmentMap.h"
#include "physically_based/PhysicallyBasedMaterial.h"
#include "physically_based/PhysicallyBasedSceneObject.h"

namespace PBR::physically_based {

class PhysicallyBasedScene : public Scene<PhysicallyBasedSceneObject> {
private:
    /**
     * The environment map of the scene
     */
    std::shared_ptr<EnvironmentMap> environmentMap;

    /**
     * The precomputed BRDF functions for each material used in the scene. Element
     * i of this vector corresponds to the ith object in the scene.
     */
    std::vector<std::shared_ptr<Texture>> brdfIntegrationMaps;

public:
    PhysicallyBasedScene(std::vector<std::shared_ptr<PhysicallyBasedSceneObject>> sceneObjects,
                         std::vector<PointLightSource> lights, std::shared_ptr<EnvironmentMap> environmentMap);

    std::shared_ptr<EnvironmentMap> getEnvironmentMap() const;

    const std::vector<std::shared_ptr<Texture>>& getBRDFIntegrationMaps() const;

private:
    void precomputeBRDFIntegrationMaps();
    std::shared_ptr<Texture> computeBRDFIntegrationMap(const PhysicallyBasedMaterial& material);
};

} // namespace PBR::physically_based

#endif //PHYSICALLYBASEDRENDERER_PHYSICALLYBASEDSCENE
