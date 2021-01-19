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
     * The prefiltered environment maps. These are specific to each object, since
     * each object may have a different BRDF configuration.
     */
    std::vector<std::shared_ptr<Texture>> prefilteredEnvironmentMaps;

    /**
     * The precomputed BRDF functions for each material used in the scene. Element
     * i of this vector corresponds to the ith object in the scene.
     */
    std::vector<std::shared_ptr<Texture>> brdfIntegrationMaps;

public:
    PhysicallyBasedScene(std::vector<std::shared_ptr<PhysicallyBasedSceneObject>> sceneObjects,
                         std::vector<PointLightSource> lights, std::shared_ptr<EnvironmentMap> environmentMap);

    std::shared_ptr<EnvironmentMap> getEnvironmentMap() const;

    const std::vector<std::shared_ptr<Texture>>& getPrefilteredEnvironmentMaps() const;

    const std::vector<std::shared_ptr<Texture>>& getBRDFIntegrationMaps() const;

private:
    /**
     * Generates the prefiltered environment map for all objects.
     */
    void precomputePrefilteredEnvironmentMaps();

    /**
     * Generates the prefiltered environment map for a given material.
     */
    std::shared_ptr<Texture> computePrefilteredEnvironmentMap(const PhysicallyBasedMaterial& material);

    /**
     * Precomputes the BRDF integration map for each object in the scene.
     */
    void precomputeBRDFIntegrationMaps();

    /**
     * Precomputes the BRDF integration map for a particular material.
     */
    std::shared_ptr<Texture> computeBRDFIntegrationMap(const PhysicallyBasedMaterial& material);
};

} // namespace PBR::physically_based

#endif //PHYSICALLYBASEDRENDERER_PHYSICALLYBASEDSCENE
