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

uniform sampler2D radianceMap;
uniform float roughness;
uniform NormalDistributionFunctionCoefficients dCoefficients;
uniform GeometricAttenuationFunctionCoefficients gCoefficients;

out vec4 FragColour;


#define PI 3.1415926535
#define EPSILON 0.000001

// Number of alpha values to choose
#define N 256


// ----- COORDINATE TRANSFORMS ----------------------------------------------------------

/**
 * Maps from Cartesian coordinates to spherical coordinates
 */
vec2 cartesianToSpherical(vec3 cartesian)
{
    float r = sqrt(cartesian.x * cartesian.x + cartesian.z * cartesian.z);
    float phi = atan(cartesian.x, -cartesian.z);
    float theta = atan(cartesian.y, r);
    return vec2(phi, theta);
}

/**
 * Maps from texture coordinates to spherical coordinates.
 */
vec2 uvToSpherical(vec2 uv)
{
    float u = uv.x;
    float v = uv.y;

    float phi = (2.0 * u - 1.0) * PI;
    float theta = (0.5 - v) * PI;

    return vec2(phi, theta);
}

/**
 * Maps from spherical coordinates to texture coordinates.
 */
vec2 sphericalToUV(vec2 phiTheta)
{
    float phi = mod(phiTheta.x + PI, 2*PI) - PI;
    float theta = mod(phiTheta.y + PI, 2*PI) - PI;

    float u = (phi + PI) / (2.0 * PI);
    float v = 0.5 - theta / PI;

    return vec2(u, v);
}

/**
 * Samples the texture using spherical coordinates.
 */
vec4 sampleTextureSpherical(vec2 spherical, float lod)
{
    vec2 uv = sphericalToUV(spherical);
    return textureLod(radianceMap, uv, lod);
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
 * Important sample the hemisphere according to the GGX probability density
 * function.
 *
 * The output will be in spherical coordinates.
 */
vec2 importanceSampleHemisphere(vec2 uv, float roughness)
{
    float u = uv.x;
    float v = uv.y;
    float a = roughness * roughness;

    float phi = 2.0 * PI * u;
    float theta = atan((a * sqrt(v)), sqrt(1 - v));

    // Correct for the fact that I've been using theta as "up" from the XZ
    // tangent plane, but this gives it in "down" from the +Y axis.
    theta = PI / 2.0 - theta;

    return vec2(phi, theta);
}

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

// ----- MAIN ---------------------------------------------------------------------------

void main()
{
    // Use a small roughness rather than zero roughness (else the importance
    // sampling algorithm goes funky!)
    float roughnessCorrected = max(roughness, 0.001);

    // Map this texture coordinate into spherical coordinates
    vec2 phiTheta = uvToSpherical(TexCoords);
    float phi = phiTheta.x;
    float theta = phiTheta.y;

    // These are the basis vectors e_phi, e_theta and e_r in Cartesian coordinates.
    // See ComputeIrradianceMap.frag for an explanation.
    vec3 n = vec3(sin(phi)*cos(theta), sin(theta), -cos(phi)*cos(theta));
    vec3 tangent = normalize(vec3(cos(phi)*cos(theta), 0, sin(phi)*cos(theta)));
    vec3 bitangent = vec3(-sin(phi)*sin(theta), cos(theta), cos(phi)*sin(theta));

    // We have to assume that we're viewing the thing from directly overhead. (Epic Games paper)
    vec3 wo = n;

    // Approximate the integral using GGX importance sampling
    vec4 result = vec4(0.0);
    float totalWeight = 0.0;
    for (int i = 0; i < N; i++) {

        // Use Hammersley point set to generate a "random" uv coordinate where
        // each of u and v are uniformly distributed in [0,1]
        vec2 uv = hammersley2D(i, N);

        // Now map that to a halfway vector h in tangent space, spherical coordinates, using
        // GGX imporance sampling
        vec2 hSphericalTangentSpace = importanceSampleHemisphere(uv, roughnessCorrected);

        // We now have (phi, theta) of our generated halfway vector h in tangent space. We need to
        // convert that to world space Cartesian coordinates so that we can use it to mirror wo to get
        // wi. We want wi because that's the direction in which we need to sample the texture, since we
        // are assuming the surface to be locally mirror-like with a local microfacet normal h. The
        // reflection of wo in h is the only possible direction from which light can come and be
        // reflected in direction wo.

        // Map h to tangent space Cartesian coordinates
        float hPhi = hSphericalTangentSpace.x;  // clockwise from -Z axis in tangent plane
        float hTheta = hSphericalTangentSpace.y; // up from tangent plane
        vec3 hTangentSpace = vec3(sin(hPhi) * cos(hTheta), sin(hTheta), -cos(hPhi) * cos(hTheta));

        // Use the basis vectors calculated earlier to map from tangent to world coordinates (Cartesian)
        vec3 h = hTangentSpace.y * n + hTangentSpace.x * tangent + hTangentSpace.z * bitangent;

        // Renormalise in case of floating-point errors
        h = normalize(h);

        // Reflect wo in h to get wi
        vec3 wi = 2.0 * dot(wo, h) * h - wo;

        // Use roughness level to choose which mipmap level to sample from. Without
        // doing this, we end up rendering "dots" for low roughness levels.
        // See: https://chetanjags.wordpress.com/2015/08/26/image-based-lighting/
        // Implementation modified from: https://learnopengl.com/PBR/IBL/Specular-IBL
        float probabilityDensity = D(n, h, roughnessCorrected);
        float pdf = (probabilityDensity * dot(n, h) / (4.0 * dot(h, wo))) + 0.0001;
        float resolution = 512.0; // Resolution of largest mipmap
        float saTexel  = 4.0 * PI / (6.0 * resolution * resolution);
        float saSample = 1.0 / (float(N) * pdf + 0.0001);
        float mipLevel = roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel);

        // Convert wi to polar spherical coordinates
        vec2 sampleCoords = sphericalToUV(cartesianToSpherical(wi));
        vec4 sampled = textureLod(radianceMap, sampleCoords, mipLevel);

        // Combine
        result += sampled * dot(n, h);  // We have to factor in the view angle for physical accuracy
        totalWeight += dot(n, h);
    }

    FragColour = result / totalWeight;
}
