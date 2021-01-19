#include "physically_based/BRDFCoefficients.h"

namespace PBR::physically_based {

bool NormalDistributionFunctionCoefficients::operator==(
        const PBR::physically_based::NormalDistributionFunctionCoefficients& other) const
{
    return this->k_TrowbridgeReitzGGX == other.k_TrowbridgeReitzGGX && this->k_Beckmann == other.k_Beckmann;
}

bool GeometricAttenuationFunctionCoefficients::operator==(const GeometricAttenuationFunctionCoefficients& other) const
{
    return this->k_SchlickGGX == other.k_SchlickGGX && this->k_CookTorrance == other.k_CookTorrance;
}

bool BRDFCoefficients::operator==(const BRDFCoefficients& other) const
{
    return this->normalDistribution == other.normalDistribution
            && this->geometricAttenutation == other.geometricAttenutation;
}

} // namespace PBR::physically_based
