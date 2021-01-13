#ifndef PHYSICALLYBASEDRENDERER_DIRECTEDLIGHTSOURCE
#define PHYSICALLYBASEDRENDERER_DIRECTEDLIGHTSOURCE

#include <glm/vec3.hpp>

namespace PBR {

/**
 * A directed light source with no identifiable position in the world
 * that illuminates all scene objects equally.
 */
struct DirectedLightSource {
    /**
     * The vector to the light source, in world space.
     */
    glm::vec3 direction;

    /**
     * The colour of the illuminant.
     */
    glm::vec3 colour;

    /**
     * The radiance that would be received if the object were 1 unit away.
     */
    float intensity;
};

} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_DIRECTEDLIGHTSOURCE
