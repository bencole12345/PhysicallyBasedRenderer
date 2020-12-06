#ifndef PHYSICALLYBASEDRENDERER_BASICPHONGSCENE
#define PHYSICALLYBASEDRENDERER_BASICPHONGSCENE

#include <set>

#include <glm/vec3.hpp>

#include "../core/LightSource.h"
#include "../core/Scene.h"

#include "PhongSceneObject.h"

namespace PBR {

class BasicPhongScene : public Scene {
private:
    std::set<std::shared_ptr<PhongSceneObject>> sceneObjects;

    std::vector<glm::vec3> lightPositions;
    std::vector<glm::vec3> lightColours;

    glm::vec3 backgroundColour;
    glm::vec3 ambientLight;

public:
    explicit BasicPhongScene(glm::vec3 backgroundColour, glm::vec3 ambientLight);

    void render(Camera& camera, double time) override;
    void activate() override;

    void addSceneObject(const std::shared_ptr<PhongSceneObject>& object);
    void addLightSource(const std::shared_ptr<LightSource>& lightSource);
};

} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_BASICPHONGSCENE
