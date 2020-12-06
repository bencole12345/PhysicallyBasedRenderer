#ifndef PHYSICALLYBASEDRENDERER_LIGHTSOURCE
#define PHYSICALLYBASEDRENDERER_LIGHTSOURCE

#include <glm/vec3.hpp>

namespace PBR {

struct LightSource {
    glm::vec3 pos;
    glm::vec3 colour;

    LightSource(glm::vec3 pos, glm::vec3 colour);
};

} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_LIGHTSOURCE
