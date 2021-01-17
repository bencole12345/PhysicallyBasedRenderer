#version 410

in vec2 TexCoords;

uniform sampler2D radianceMap;
uniform float roughness;

out vec4 FragColour;


#define PI 3.14159

// Number of alpha values to choose
#define N 256


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

/**
 * The GGX probability density function.
 */
float distributionGGX(vec3 n, vec3 h)
{
    float a = roughness * roughness;
    float numerator = a * a;
    float denominator = PI * pow(dot(n, h), 4) * pow((pow(dot(n, h), 2.0) * (a*a - 1.0) + 1.0), 2.0);
    return numerator / denominator;
}

void main()
{
    // Use a small roughness rather than zero roughness
    float roughnessCorrected = max(roughness, 0.001);

    // Map this texture coordinate into spherical coordinates
    vec2 phiTheta = uvToSpherical(TexCoords);
    float phi = phiTheta.x;
    float theta = phiTheta.y;

    // Compute the basis vectors e_phi, e_theta, e_r in Cartesian coordinates.
    // These are the tangent, bitangent and normal vectors respectively.
    // Derivation: write the expression for mapping from spherical polar to Cartesian
    // coordinates then differentiate wrt phi, theta and r respectively
    vec3 e_phi = normalize(vec3(cos(phi)*cos(theta), 0, sin(phi)*cos(theta)));
    vec3 e_theta = vec3(-sin(phi)*sin(theta), cos(theta), cos(phi)*sin(theta));
    vec3 e_r = vec3(sin(phi)*cos(theta), sin(theta), -cos(phi)*cos(theta));

    // Rename so it makes more sense later on
    vec3 n = vec3(sin(phi)*cos(theta), sin(theta), -cos(phi)*cos(theta));
    vec3 tangent = normalize(vec3(cos(phi)*cos(theta), 0, sin(phi)*cos(theta)));
    vec3 bitangent = vec3(-sin(phi)*sin(theta), cos(theta), cos(phi)*sin(theta));

    // We have to assume that we're viewing the thing from directly overhead.
    vec3 v = n;

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
        // convert that to world space Cartesian coordinates so that we can use it to mirror v to get
        // l. We want l because that's the direction in which we need to sample the texture.

        // Map h to tangent space Cartesian coordinates
        float hPhi = hSphericalTangentSpace.x;  // clockwise from -Z axis in tangent plane
        float hTheta = hSphericalTangentSpace.y; // up from tangent plane
        vec3 hTangentSpace = vec3(sin(hPhi) * cos(hTheta), sin(hTheta), -cos(hPhi) * cos(hTheta));

        // Use the basis vectors calculated earlier to map from tangent to world coordinates (Cartesian)
        vec3 h = hTangentSpace.y * n + hTangentSpace.x * tangent + hTangentSpace.z * bitangent;

        // Renormalise in case of floating-point errors
        h = normalize(h);

        // Reflect v in h to get l
        vec3 l = 2.0 * dot(v, h) * h - v;

        // Use roughness level to choose which mipmap level to sample from. Motivation: without
        // doing this, we end up rendering "dots".
        // See: https://chetanjags.wordpress.com/2015/08/26/image-based-lighting/
        // Implementation modified from: https://learnopengl.com/PBR/IBL/Specular-IBL
        float probabilityDensity = distributionGGX(n, h);
        float pdf = (probabilityDensity * dot(n, h) / (4.0 * dot(h, v))) + 0.0001;
        float resolution = 512.0; // Resolution of largest mipmap
        float saTexel  = 4.0 * PI / (6.0 * resolution * resolution);
        float saSample = 1.0 / (float(N) * pdf + 0.0001);
        float mipLevel = roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel);

        // Convert l to polar spherical coordinates
        vec2 sampleCoords = sphericalToUV(cartesianToSpherical(l));
        vec4 sampled = textureLod(radianceMap, sampleCoords, mipLevel);

        // Combine
        result += sampled * dot(n, h);
        totalWeight += dot(n, h);
    }

    FragColour = result / totalWeight;
}
