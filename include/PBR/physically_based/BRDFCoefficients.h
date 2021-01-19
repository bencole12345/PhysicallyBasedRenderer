#ifndef PHYSICALLYBASEDRENDERER_BRDFCOEFFICIENTS
#define PHYSICALLYBASEDRENDERER_BRDFCOEFFICIENTS

namespace PBR::physically_based {

/**
 * Selects how much to use of each normal distribution function.
 */
struct NormalDistributionFunctionCoefficients {
    float k_TrowbridgeReitzGGX;
    float k_Beckmann;

    bool operator==(const struct NormalDistributionFunctionCoefficients& other) const;
};

/**
 * Selects how much to use of each geometric attenuation function.
 */
struct GeometricAttenuationFunctionCoefficients {
    float k_SchlickGGX;
    float k_CookTorrance;

    bool operator==(const struct GeometricAttenuationFunctionCoefficients& other) const;
};

/**
 * Specifies the proportion of each constituent function used to compute the Cook-Torrance BRDF.
 */
struct BRDFCoefficients {
    NormalDistributionFunctionCoefficients normalDistribution;
    GeometricAttenuationFunctionCoefficients geometricAttenutation;

    bool operator==(const struct BRDFCoefficients& other) const;
};

} // namespace PBR::physically_based

#endif //PHYSICALLYBASEDRENDERER_BRDFCOEFFICIENTS
