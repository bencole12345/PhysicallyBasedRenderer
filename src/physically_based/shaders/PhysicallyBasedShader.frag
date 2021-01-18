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

/**
 * Contains information about the scene's sun.
 */
struct SunInfo {
    vec3 direction;
    vec3 colour;
    float intensity;
};


in vec4 Normal;
in vec4 Position_world;
in vec2 TexCoord;

uniform vec3 cameraPosition;
uniform Material material;
uniform DirectLightingInfo lightingInfo;
uniform SunInfo sunInfo;

uniform sampler2D irradianceMap;
uniform sampler2D preFilteredEnvironmentMap;
uniform sampler2D brdfIntegrationMap;

out vec4 FragColour;


#define PI 3.14159
#define EPSILON 0.000001


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
    return G_SchlickGGX(n, v, k) * G_SchlickGGX(n, l, k);
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

vec3 fresnelSchlickRoughness(vec3 h, vec3 v, vec3 F0, float roughness)
{
    float cosTheta = dot(h, v);
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

vec3 BRDF(vec3 p, vec3 wi, vec3 wo, vec3 n, vec3 F0)
{
    // Compute h (halfway) vector
    vec3 h = normalize(wi + wo);

    // Compute the three functions to determine the specular term
    float D = normalDistribution(n, h, material.roughness);
    float G = geometry(n, wo, wi, material.roughness);
    vec3 F = fresnelSchlickRoughness(h, wo, F0, material.roughness);

    // Use F and the material's metallic level to work out the diffuse coefficient
    vec3 kS = F;
    vec3 kD = (vec3(1.0) - kS) * (1 - material.metallic);

    // Compute the coefficient to be applied to the radiance from this light
    vec3 numerator = D * G * F;
    float denominator = 4 * max(dot(n, wo), 0.0) * max(dot(n, wi), 0.0);
    vec3 specular = numerator / max(denominator, 0.001);

    return (kD * material.albedo / PI) + specular;
}

/**
 * Computes the additive contribution from a particular light in the set of
 * point lights.
 */
vec3 contributionFromLight(int lightIndex, vec3 n, vec3 p, vec3 v, vec3 F0)
{
    // Read data about this light
    vec3 p_light = lightingInfo.lightPositions[lightIndex];
    vec3 luminance = lightingInfo.intensities[lightIndex] * lightingInfo.lightColours[lightIndex];

    // Work out how much energy we receive from this light source
    float attenuationAmount = attenuation(p_light, p);
    vec3 radiance = attenuationAmount * luminance;

    // Vector to the light
    vec3 l = normalize(p_light - p);

    return BRDF(p, l, v, n, F0) * radiance * max(dot(n, l), 0.0);
}

/**
 * Computes the contribution from the sun.
 */
vec3 contributionFromSun(vec3 n, vec3 p, vec3 v, vec3 F0)
{
    vec3 radiance = sunInfo.colour * sunInfo.intensity;
    vec3 l = sunInfo.direction;
    return BRDF(p, l, v, n, F0) * radiance * max(dot(n, l), 0.0);
}

/**
 * Convert from cubemap coordinates to UV coordinates for the texture.
 */
vec2 cubemapCoordsToUVs(vec3 cubemapCoords)
{
    // Radius when projected into the XZ plane
    float r = sqrt(cubemapCoords.x * cubemapCoords.x + cubemapCoords.z * cubemapCoords.z);

    // Rotation clockwise from -Z axis
    float phi = atan(cubemapCoords.x, -cubemapCoords.z);

    // Rotation up from XZ plane
    float theta = atan(cubemapCoords.y, r);

    // Corresponding texture location
    float u = 0.5 + phi / (2 * PI);
    float v = 0.5 + theta / PI;

    return vec2(u, v);
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
    vec3 F0_corrected = mix(material.albedo, material.F0, material.metallic);

    // Add the contributions of all point light sources
    vec3 Lo = vec3(0.0);
    for (int i = 0; i < MAX_LIGHTS; i++) {
        Lo += contributionFromLight(i, n, p, v, F0_corrected);
    }

    // Add the contribution from the sun
    Lo += contributionFromSun(n, p, v, F0_corrected);

    // Work out kD coefficient
    vec3 F = fresnelSchlickRoughness(n, v, F0_corrected, material.roughness);
    vec3 kD = (1.0 - F) * (1.0 - material.metallic);

    // Add the diffuse contribution from the irradiance map
    vec2 uv = cubemapCoordsToUVs(n);
    vec4 sampledDiffuse = texture(irradianceMap, uv);
    Lo += kD * sampledDiffuse.rgb * material.albedo;

    // Compute the incoming specular light direction
    vec3 l = 2.0 * dot(v, n) * n - v;

    // Sample the precomputed environment map and BRDF function
    float lod = material.roughness * 4.0;
    vec3 environmentMapComponent = textureLod(preFilteredEnvironmentMap, cubemapCoordsToUVs(l), lod).rgb;
    vec4 brdfScaleAndBias = texture(brdfIntegrationMap, vec2(max(dot(v, n), 0.0), material.roughness));
    float F0_scale = brdfScaleAndBias.x;
    float F0_bias = brdfScaleAndBias.y;

    // Work out the specular contribution
    Lo += environmentMapComponent * (F0_scale * F + F0_bias);

    // Correct the output colour
    vec3 colour = toneMap(Lo);
    vec3 gammaEncoded = gammaEncode(colour);

    FragColour = vec4(gammaEncoded, 1.0f);
}
