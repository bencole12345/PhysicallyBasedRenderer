#ifndef PHYSICALLYBASEDRENDERER_POINTLIGHTSOURCE
#define PHYSICALLYBASEDRENDERER_POINTLIGHTSOURCE

#include <glm/vec3.hpp>

namespace PBR {

struct PointLightSource {
    glm::vec3 pos;
    glm::vec3 colour;

    PointLightSource(glm::vec3 pos, glm::vec3 colour);
};

} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_POINTLIGHTSOURCE
