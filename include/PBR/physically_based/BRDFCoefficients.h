#ifndef PHYSICALLYBASEDRENDERER_BRDFCOEFFICIENTS
#define PHYSICALLYBASEDRENDERER_BRDFCOEFFICIENTS

namespace PBR::physically_based {

/**
 * Selects how much to use of each normal distribution function.
 */
struct NormalDistributionFunctionCoefficients {
    float k_TrowbridgeReitzGGX;
    float k_Beckman;
};

/**
 * Selects how much to use of each geometric attenuation function.
 */
struct GeometricAttenuationFunctionCoefficients {
    float k_SchlickGGX;
    float k_CookTorrance;
};

/**
 * Specifies the proportion of each constituent function used to compute the Cook-Torrance BRDF.
 */
struct BRDFCoefficients {
    NormalDistributionFunctionCoefficients normalDistribution;
    GeometricAttenuationFunctionCoefficients geometricAttenutation;
};

} // namespace PBR::physically_based

#endif //PHYSICALLYBASEDRENDERER_BRDFCOEFFICIENTS
