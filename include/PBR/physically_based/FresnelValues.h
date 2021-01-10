#ifndef PHYSICALLYBASEDRENDERER_FRESNELVALUES
#define PHYSICALLYBASEDRENDERER_FRESNELVALUES

#include <glm/vec3.hpp>

namespace PBR::physically_based {

/**
 * Contains a library of F0 values for common materials.
 * 
 * Source: Naty Hoffman's course on physically based shading,
 * https://blog.selfshadow.com/publications/s2013-shading-course/hoffman/s2013_pbs_physics_math_notes.pdf
 */
struct FresnelValues {
    static constexpr glm::vec3 Water = glm::vec3(0.02f, 0.02f, 0.02f);
    static constexpr glm::vec3 PlasticLow = glm::vec3(0.03f, 0.03f, 0.03f);
    static constexpr glm::vec3 PlasticHigh = glm::vec3(0.05f, 0.05f, 0.05f);
    static constexpr glm::vec3 GlassLow = glm::vec3(0.03f, 0.03f, 0.03f);
    static constexpr glm::vec3 GlassHigh = glm::vec3(0.08f, 0.08f, 0.08f);
    static constexpr glm::vec3 Diamond = glm::vec3(0.17f, 0.17f, 0.17f);
    static constexpr glm::vec3 Iron = glm::vec3(0.56f, 0.57f, 0.58f);
    static constexpr glm::vec3 Copper = glm::vec3(0.95f, 0.64f, 0.54f);
    static constexpr glm::vec3 Gold = glm::vec3(1.00f, 0.71f, 0.29f);
    static constexpr glm::vec3 Aluminium = glm::vec3(0.91f, 0.92f, 0.92f);
    static constexpr glm::vec3 Silver = glm::vec3(0.95f, 0.93f, 0.88f);
};

} // namespace PBR::physically_based

#endif //PHYSICALLYBASEDRENDERER_FRESNELVALUES
