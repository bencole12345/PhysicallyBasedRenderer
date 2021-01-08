#ifndef PHYSICALLYBASEDRENDERER_PHYSICALLYBASEDMATERIAL
#define PHYSICALLYBASEDRENDERER_PHYSICALLYBASEDMATERIAL

#include "glm/vec3.hpp"

namespace PBR::physically_based {

struct PhysicallyBasedMaterial {
    glm::vec3 albedo;
    float roughness;
    float metallic;
    glm::vec3 F0;
};

} // namespace PBR::physically_based

#endif //PHYSICALLYBASEDRENDERER_PHYSICALLYBASEDMATERIAL
