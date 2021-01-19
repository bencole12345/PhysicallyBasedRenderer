#ifndef PHYSICALLYBASEDRENDERER_PHONGSCENE
#define PHYSICALLYBASEDRENDERER_PHONGSCENE

#include <memory>
#include <optional>
#include <vector>

#include <glm/vec3.hpp>

#include "core/PointLightSource.h"
#include "core/Scene.h"
#include "phong/PhongSceneObject.h"
#include "phong/Skybox.h"

namespace PBR::phong {

class PhongScene : public Scene<PhongSceneObject> {
private:
    /**
     * The ambient light term to be used in the Phong shading model.
     */
     glm::vec3 ambientLight;

    /**
     * The skybox. Will only be rendered if a value has been provided.
     */
    std::optional<std::shared_ptr<Skybox>> skybox;

public:
    PhongScene(std::vector<std::shared_ptr<PhongSceneObject>> sceneObjects, glm::vec3 ambientLight,
               std::vector<PointLightSource> lights, glm::vec3 backgroundColour);

    PhongScene(std::vector<std::shared_ptr<PhongSceneObject>> sceneObjects, glm::vec3 ambientLight,
               std::vector<PointLightSource> lights, std::shared_ptr<Skybox> skybox);

    glm::vec3 getAmbientLight() const;

    bool hasSkybox() const;
    const std::shared_ptr<Skybox>& getSkybox() const;
};

} // namespace PBR::phong

#endif //PHYSICALLYBASEDRENDERER_PHONGSCENE
