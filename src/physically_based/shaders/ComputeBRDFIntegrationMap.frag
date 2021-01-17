#version 410


in vec2 TexCoords;

out vec4 FragColour;


#define PI 3.14159
#define EPSILON 0.000001

// Number of samples to use
#define N 512


/**
 * Maps roughness values to alpha values.
 */
float alpha(float roughness)
{
    return roughness;
}

/**
 * A normal distribution function for computing how normals are spread.
 *
 * This implementation uses the GGX model.
 */
float normalDistribution(vec3 n, vec3 h, float roughness)
{
    float a = alpha(roughness);
    float k = a * a / 2.0f; // use the mapping for IBL rather than direct

    // Compute the formula
    float numerator = a * a;
    float n_dot_h = dot(n, h);
    float denominatorInner = (n_dot_h * n_dot_h * (a * a - 1.0) + 1.0);
    float denominatorFull = PI * denominatorInner * denominatorInner;

    // Make sure we don't accidentally divide by zero
    float denominator = max(denominatorFull, EPSILON);

    return numerator / denominator;
}

/**
 * A geometry function for computing self-shadowing of microfaceted surfaces.
 *
 * Note that this function only computes the self-shadowing factor in one
 * direction.
 */
float G_SchlickGGX(vec3 n, vec3 v, float k)
{
    return dot(n, v) / (dot(n, v) * (1.0 - k) + k);
}

/**
 * A full geometry function for microfacet self-shadowing that factors in
 * self-shadowing both from the light source and to the viewer.
 */
float geometry(vec3 n, vec3 v, vec3 l, float roughness)
{
    float a = alpha(roughness);
    float k = a * a / 2.0f;  // formula for image-based mapping
    return G_SchlickGGX(n, v, k) * G_SchlickGGX(n, l, k);
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
    float a = alpha(roughness);

    float phi = 2.0 * PI * u;
    float theta = atan((a * sqrt(v)), sqrt(1 - v)); // From the paper

    // Correct for the fact that I've been using theta as "up" from the XZ
    // tangent plane, but this gives it in "down" from the +Y axis.
    theta = PI / 2.0 - theta;

    return vec2(phi, theta);
}

void main()
{
    float n_dot_w0 = TexCoords.x;
    float roughness = TexCoords.y;

    // We work in tangent space, meaning that (arbitrary decision) the X and Z axes
    // are considered tangential to the surface while the Y axis is parallel to the
    // local normal.

    // First work out out an appropriate view vector to use, using the arbitrary
    // decision that phi=0 in world space
    vec3 v = vec3(0, n_dot_w0, sin(acos(n_dot_w0)));

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

        // Reflect v in h to get l.
        vec3 l = normalize(2 * dot(v, h) * h - v);

        // Ignore any values that can't contribute any light
        if (dot(n, l) <= 0)
            continue;

        // We now have everything we need to sample the BDRF
        float G = (geometry(n, v, l, roughness) * dot(v, h)) / (dot(n, h) * dot(n, v));
        float schlick = pow(1.0 - dot(v, h), 5.0);

        scale += G * (1.0 - schlick);
        bias += G * schlick;
    }

    FragColour = vec4(scale/N, bias/N, 0.0, 1.0);
}
