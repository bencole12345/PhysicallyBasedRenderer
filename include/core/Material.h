#ifndef PHYSICALLYBASEDRENDERER_MATERIAL
#define PHYSICALLYBASEDRENDERER_MATERIAL

namespace PBR {

/**
 * Captures information about an object's material.
 */
struct Material {
    float kD;
    float kS;
    float specularN;
};

} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_MATERIAL
