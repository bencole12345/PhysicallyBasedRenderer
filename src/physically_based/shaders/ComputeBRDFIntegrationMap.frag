#version 410


struct NormalDistributionFunctionCoefficients {
    float k_TrowbridgeReitzGGX;
    float k_Beckmann;
};

struct GeometricAttenuationFunctionCoefficients {
    float k_SchlickGGX;
    float k_CookTorrance;
};


in vec2 TexCoords;

uniform NormalDistributionFunctionCoefficients dCoefficients;
uniform GeometricAttenuationFunctionCoefficients gCoefficients;

out vec4 FragColour;


#define PI 3.1415926535
#define EPSILON 0.000001

// Number of samples to use
#define N 512


// ----- NORMAL DISTRIBUTION FUNCTION ---------------------------------------------------

/**
 * The Trowbridge-Reitz GGX normal distribution function.
 *
 * Adapted from LearnOpenGL book.
 */
float D_TrowbridgeReitzGGX(vec3 n, vec3 h, float roughness)
{
    float alpha = roughness * roughness;  // Square roughness for direct lighting
    float n_dot_h = max(dot(n, h), 0.0);

    // Compute the formula
    float numerator = alpha * alpha;
    float denominator = PI * pow((n_dot_h * n_dot_h * (alpha * alpha - 1.0) + 1.0), 2);

    return numerator / max(denominator, EPSILON);
}

/**
 * The Beckmann normal distribution function.
 *
 * Implementation adapted from: https://www.jordanstevenstechart.com/physically-based-rendering
 */
float D_Beckmann(vec3 n, vec3 h, float roughness)
{
    float alpha = roughness * roughness;  // Square roughness for direct lighting
    float n_dot_h = dot(n, h);
    return max(EPSILON, (1.0 / (PI * alpha * pow(n_dot_h, 4)))
    * exp((pow(n_dot_h, 2) - 1)/(alpha * pow(n_dot_h, 2))));
}

/**
 * The interface to the normal distribution function.
 *
 * This mixes the different implementations according to the values in
 * gCoefficients.
 */
float D(vec3 n, vec3 h, float roughness)
{
    return dCoefficients.k_TrowbridgeReitzGGX * D_TrowbridgeReitzGGX(n, h, roughness)
    + dCoefficients.k_Beckmann * D_Beckmann(n, h, roughness);
}

// ----- GEOMETRIC ATTENUATION FUNCTION -------------------------------------------------

/**
 * A geometry function for computing self-shadowing of microfaceted surfaces.
 *
 * Note that this function only computes the self-shadowing factor in one
 * direction.
 *
 * Adapted from implementation in LearnOpenGL book.
 */
float G_SchlickGGX(vec3 n, vec3 wo, float k)
{
    return dot(n, wo) / (dot(n, wo) * (1 - k) + k);
}

/**
 * Schlick GGX geometry function using Schlick's method.
 *
 * Adapted from LearnOpenGL book.
 */
float G_Smith_SchlickGGX(vec3 n, vec3 wo, vec3 wi, float roughness)
{
    float alpha = roughness * roughness;
    float k = (alpha + 1.0) * (alpha + 1.0) / 8.0;  // Formula for direct lighting
    return G_SchlickGGX(n, wo, k) * G_SchlickGGX(n, wi, k);
}

/**
 * The Cook-Torrance geometry function.
 *
 * Implemented using the formula from the original paper.
 */
float G_CookTorrance(vec3 n, vec3 wo, vec3 wi)
{
    vec3 h = normalize(wo + wi);
    float first = 2.0 * dot(n, h) * dot(n, wo) / dot(wo, h);
    float second = 2.0 * dot(n, h) * dot(n, wi) / dot(wo, h);
    return min(min(first, second), 1.0);
}

/**
 * The interface to the geometric attenuation function.
 *
 * This mixes the different implementations according to the values in
 * gCoefficients.
 */
float G(vec3 n, vec3 wo, vec3 wi, float roughness)
{
    return gCoefficients.k_SchlickGGX * G_Smith_SchlickGGX(n, wo, wi, roughness)
         + gCoefficients.k_CookTorrance * G_CookTorrance(n, wo, wi);
}

// ----- IMPORTANCE SAMPLING ------------------------------------------------------------

/**
 * Given an integer `bits` written in fixed-point binary decimal notation,
 * this mirrors the binary digits about the decimal point to get a value
 * roughly uniformly distributed between 0 and 1.
 *
 * Example:
 *      5 (base 10) = 101.0   (base 2)
 *                 ->   0.101 (base 2)
 *                  = 0.625   (base 10)
 *
 * DISCLAIMER: I DIDN'T WRITE THIS FUNCTION!
 *     SOURCE: http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
 */
float radicalInverse_VdC(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

/**
 * Uses the radicalInverse function to generate a series of points that
 * are roughly uniformly distributed in [0, 1] x [0, 1].
 *
  * DISCLAIMER: I DIDN'T WRITE THIS FUNCTION!
 *      SOURCE: adapted from http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
 */
vec2 hammersley2D(uint i, uint N_total)
{
    return vec2(float(i)/float(N_total), radicalInverse_VdC(i));
}

/**
 * Importance sample the hemisphere according to the GGX probability density
 * function.
 *
 * This uses the mapping described in the original GGX paper, "Microfacet Models
 * for Refraction through Rough Surfaces". The output will be in spherical coordinates.
 */
vec2 importanceSampleHemisphere(vec2 uv, float roughness)
{
    float u = uv.x;
    float v = uv.y;
    float a = roughness;  // Roughness directly scales the distribution

    // See paper for explanation. This is derived from the CDF of the GGX
    // probability distribution.
    float phi = 2.0 * PI * u;
    float theta = atan((a * sqrt(v)), sqrt(1 - v));

    // Correct for the fact that I've been using theta as "up" from the XZ
    // tangent plane, but this gives it in "down" from the +Y axis.
    theta = PI / 2.0 - theta;

    return vec2(phi, theta);
}

void main()
{
    // These are just how the lookup function is indexed.
    float n_dot_w0 = TexCoords.x;
    float roughness = TexCoords.y;

    // We work in tangent space, meaning that (arbitrary decision:) the X and Z axes
    // are considered tangential to the surface while the Y axis is parallel to the
    // local normal. However, unlike other times in this application where we sample
    // the hemisphere, we never actually need to map into world coordinates here, since
    // the BRDF integration map is determined entirely by the BRDF in use, not by
    // the environment map. Hence, it's the same regardless of which environment map
    // and orientation you choose.

    // First work out out an appropriate view vector to use, using the arbitrary
    // decision that phi=0 in world space
    vec3 wo = vec3(0, n_dot_w0, sin(acos(n_dot_w0)));

    // We also assume that the normal is (0, 1, 0)
    vec3 n = vec3(0.0, 1.0, 0.0);

    // We're going to compute both integrals in the same loop. The idea here is
    // to sample a linearly spaced sequence of h values along the unit circle
    // from (1, 0, 0) clockwise round to (-1, 0, 0). Rather than using
    // importance sampling, I multiply each contribution by the D value at that
    // point to ensure that this is an unbiased sample.
    float scale = 0;
    float bias = 0;

    float totalProbability = 0;

    for (int i = 0; i < N; i++) {

        // Use Hammersley point set to generate a "random" uv coordinate where
        // each of u and v are uniformly distributed in [0,1]
        vec2 uv = hammersley2D(i, N);

        // Now map that to a halfway vector h using GGX imporance sampling,
        // so regions with a high GGX probability density value are more likely
        // to be selected
        vec2 hSphericalCoordinates = importanceSampleHemisphere(uv, roughness);

        // Map to tangent space Cartesian coordinates
        float hPhi = hSphericalCoordinates.x;  // clockwise from -Z axis in tangent plane
        float hTheta = hSphericalCoordinates.y; // up from tangent plane
        vec3 h = vec3(sin(hPhi) * cos(hTheta), sin(hTheta), -cos(hPhi) * cos(hTheta));

        // Mathematically this should be normalised, but floating-point errors are a thing, so
        // let's renormalise just to be safe.
        h = normalize(h);

        // Reflect wo in h to get wi.
        vec3 wi = normalize(2 * dot(wo, h) * h - wo);

        // Ignore any values that can't contribute any light
        if (dot(n, wi) <= 0)
            continue;

        // We now have everything we need to sample the BDRF. This hefty formula is
        // just an implementation of equation 8 in Epic Games' paper.
        // See: https://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf
        float commonPart = (G(n, wo, wi, roughness) * dot(wo, h)) / (dot(n, h) * dot(n, wo));
        float schlickPart = pow(1.0 - dot(wo, h), 5.0);

        scale += commonPart * (1.0 - schlickPart);
        bias += commonPart * schlickPart;
    }

    FragColour = vec4(scale/N, bias/N, 0.0, 1.0);
}
