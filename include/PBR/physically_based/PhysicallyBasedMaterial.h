#ifndef PHYSICALLYBASEDRENDERER_PHYSICALLYBASEDMATERIAL
#define PHYSICALLYBASEDRENDERER_PHYSICALLYBASEDMATERIAL

#include <glm/vec3.hpp>

namespace PBR::physically_based {

/**
 * Encodes the information required to describe a physically-based material.
 */
struct PhysicallyBasedMaterial {
    glm::vec3 albedo;
    float roughness;
    float metallic;
    glm::vec3 F0;

    // Implemented so that we can hash it
    bool operator==(const struct PhysicallyBasedMaterial& other) const
    {
        return this->albedo == other.albedo
                && this->roughness == other.roughness
                && this->metallic == other.metallic
                && this->F0 == other.F0;
    }
};

} // namespace PBR::physically_based

#endif //PHYSICALLYBASEDRENDERER_PHYSICALLYBASEDMATERIAL
