#ifndef PHYSICALLYBASEDRENDERER_PHONGSCENE
#define PHYSICALLYBASEDRENDERER_PHONGSCENE

#include <set>

#include <glm/vec3.hpp>

#include "core/PointLightSource.h"
#include "phong/PhongSceneObject.h"

namespace PBR {
namespace phong {

class PhongScene {
private:
    std::set<std::shared_ptr<PhongSceneObject>> sceneObjects;

    std::vector<glm::vec3> lightPositions;
    std::vector<glm::vec3> lightColours;

    glm::vec3 backgroundColour;
    glm::vec3 ambientLight;

public:
    explicit PhongScene(glm::vec3 backgroundColour, glm::vec3 ambientLight);

    void render(const Camera& camera, double time);
    void activate();

    void addSceneObject(const std::shared_ptr<PhongSceneObject>& object);
    void addLightSource(const std::shared_ptr<PointLightSource>& lightSource);
};

} // namespace phong
} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_PHONGSCENE
