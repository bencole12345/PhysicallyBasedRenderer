#ifndef PHYSICALLYBASEDRENDERER_PHONGMATERIAL
#define PHYSICALLYBASEDRENDERER_PHONGMATERIAL

#include <optional>

#include <glm/vec3.hpp>

namespace PBR::phong {

/**
 * Captures information about an object's material.
 */
struct PhongMaterial {
    float kD;
    float kS;
    float specularN;

    /**
     * If no colour is supplied then it assumed that the object is textured instead.
     */
    std::optional<glm::vec3> colour;
};

} // namespace PBR::phong

#endif //PHYSICALLYBASEDRENDERER_PHONGMATERIAL
