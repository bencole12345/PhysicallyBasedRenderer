#version 410 core

#define MAX_LIGHTS 16

/**
 * Contains the parameters needed to describe a material in this physically
 * based framework.
 */
struct Material {
    vec3 albedo;
    float roughness;
    float metallic;
    vec3 F0;
};

/**
 * Contains information about all point light sources in the scene.
 */
struct DirectLightingInfo {
    vec3 lightPositions[MAX_LIGHTS];
    vec3 lightColours[MAX_LIGHTS];
    float intensities[MAX_LIGHTS];
};

in vec4 Normal;
in vec4 Position_world;
in vec2 TexCoord;

uniform vec3 cameraPosition;
uniform Material material;
uniform DirectLightingInfo lightingInfo;
uniform samplerCube skybox;

out vec4 FragColor;


const float PI = 3.14159;
const float EPSILON = 0.000001;


/**
 * Computes the attenutation of light according to the inverse-square law.
 */
float attenuation(vec3 from, vec3 to)
{
    float dist = length(to - from);
    return 1 / (dist * dist);
}

/**
 * Maps roughness values to alpha values.
 */
float alpha(float roughness)
{
    return roughness * roughness;
}

/**
 * Maps alpha values to the K parameter needed for the G_SchlickGGX geometry
 * function.
 */
float K(float alpha, bool imageBased)
{
    if (imageBased) {
        return alpha * alpha / 2.0f;
    }
    else {
        return (alpha + 1.0) * (alpha + 1.0) / 8.0;
    }
}

/**
 * A normal distribution function for computing how normals are spread.
 *
 * This implementation uses the GGX model.
 */
float normalDistribution(vec3 n, vec3 h, float roughness)
{
    float a = alpha(roughness);
    float n_dot_h = max(dot(n, h), 0.0);

    // Compute the formula
    float numerator = a * a;
    float denominatorInner = (n_dot_h * n_dot_h * (a * a - 1.0) + 1.0);
    float denominatorFull = 3.14159 * denominatorInner * denominatorInner;

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
    return dot(n, v) / (dot(n, v) * (1 - k) + k);
}

/**
 * A full geometry function for microfacet self-shadowing that factors in
 * self-shadowing both from the light source and to the viewer.
 */
float geometry(vec3 n, vec3 v, vec3 l, float roughness)
{
    float a = alpha(roughness);
    float k = K(a, false);
//    return G_SchlickGGX(n, v, k) * G_SchlickGGX(n, l, k);
    return G_SchlickGGX(n, v, k);
}

/**
 * Approximate the Fresnel reflectivity of the material from this viewing
 * angle.
 */
vec3 fresnel(vec3 h, vec3 v, vec3 F0)
{
    float cosTheta = max(dot(h, v), 0.0);
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

/**
 * Apply tone mapping to a HDR colour to make it representible in regular
 * colours.
 */
vec3 toneMap(vec3 colourHDR)
{
    return colourHDR / (vec3(1.0) + colourHDR);
}

/**
 * Gamma encode a colour in linear colour space.
 */
vec3 gammaEncode(vec3 colour)
{
    return pow(colour, vec3(1.0 / 2.2));
}

void main()
{
    // Shorter names for the vectors
    vec3 n = Normal.xyz;
    vec3 p = Position_world.xyz;
    vec3 v = normalize(cameraPosition - p);

    // Base reflectivity: we have to correct for the fact that this only really
    // makes sense for non-metals. For metals, we instead use the material's
    // albedo as F0.
    vec3 F0_corrected = mix(material.F0, material.albedo, material.metallic);

    // Integrate by summing over all light sources
    vec3 Lo = vec3(0.0);
    for (int i = 0; i < MAX_LIGHTS; i++) {

        // Work out how much energy we receive from this light source,
        // factoring in its attenuation.
        vec3 p_light = lightingInfo.lightPositions[i];
        vec3 luminance = lightingInfo.intensities[i] * lightingInfo.lightColours[i];
        float attenuationAmount = attenuation(p_light, p);
        vec3 radiance = attenuationAmount * luminance;

        // Compute l (to light) and h (halfway) vectors
        vec3 l = normalize(p_light - p);
        vec3 h = normalize(v + l);

        // Compute the three functions to determine the specular term
        float D = normalDistribution(n, h, material.roughness);
        float G = geometry(n, v, l, material.roughness);
        vec3 F = fresnel(h, v, F0_corrected);

        // Use F and the material's metallic level to work out the diffuse coefficient
        vec3 kS = F;
        vec3 kD = (vec3(1.0) - kS) * (1 - material.metallic);

        // Compute the coefficient to be applied to the radiance from this light
        vec3 numerator = D * G * F;
        float denominator = 4 * max(dot(n, v), 0.0) * max(dot(n, l), 0.0);
        vec3 specular = numerator / max(denominator, 0.001);
        vec3 coefficientThisLight = (kD * material.albedo / PI) + specular;

        // Work out the (additive) contribution from this light
        vec3 contributionFromThisLight = coefficientThisLight * radiance * max(dot(n, l), 0.0);

        // Add it to the accumulator for the integral
        Lo += contributionFromThisLight;
    }

    // Add some ambient lighting
    Lo += 0.03 * material.albedo;

    // Correct the output colour
    vec3 colour = toneMap(Lo);
    vec3 gammaEncoded = gammaEncode(colour);

    FragColor = vec4(gammaEncoded, 1.0f);
}
