#include "physically_based/PhysicallyBasedMaterial.h"

namespace PBR::physically_based {

bool PhysicallyBasedMaterial::operator==(
        const PBR::physically_based::PhysicallyBasedMaterial& other) const
{
    return this->albedo == other.albedo
            && this->roughness == other.roughness
            && this->metallic == other.metallic
            && this->F0 == other.F0
            && this->brdfCoefficients == other.brdfCoefficients;
}

} // namespace PBR::physically_based
