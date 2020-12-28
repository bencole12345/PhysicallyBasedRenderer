#ifndef PHYSICALLYBASEDRENDERER_PHONGSCENE
#define PHYSICALLYBASEDRENDERER_PHONGSCENE

#include <memory>
#include <optional>
#include <vector>

#include <glm/vec3.hpp>

#include "core/PointLightSource.h"
#include "core/Skybox.h"
#include "phong/PhongSceneObject.h"

namespace PBR {
namespace phong {

class PhongScene {
private:
    std::vector<std::shared_ptr<PhongSceneObject>> sceneObjects;

    std::vector<glm::vec3> lightPositions;
    std::vector<glm::vec3> lightColours;

    glm::vec3 backgroundColour;
    glm::vec3 ambientLight;

    std::optional<std::shared_ptr<Skybox>> skybox;

public:
    explicit PhongScene(glm::vec3 backgroundColour, glm::vec3 ambientLight);

    void render(const Camera& camera, double time);
    void activate();

    void addSceneObject(const std::shared_ptr<PhongSceneObject>& object);
    void addLightSource(const std::shared_ptr<PointLightSource>& lightSource);

    void addSkybox(std::shared_ptr<Skybox> skybox);
    void removeSkybox();
};

} // namespace phong
} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_PHONGSCENE
