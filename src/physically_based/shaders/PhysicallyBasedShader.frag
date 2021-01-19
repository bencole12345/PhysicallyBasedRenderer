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

/**
 * Specifies the proportion of each normal distribution to use.
 */
struct NormalDistributionFunctionCoefficients {
    float k_TrowbridgeReitzGGX;
    float k_Beckmann;
};

/**
 * Specifies the proportion of each geometric attenuation function to use.
 */
struct GeometricAttenuationFunctionCoefficients {
    float k_SchlickGGX;
    float k_CookTorrance;
};


in vec4 Normal;
in vec4 Position_world;
in vec2 TexCoord;

uniform vec3 cameraPosition;
uniform Material material;
uniform DirectLightingInfo lightingInfo;
uniform SunInfo sunInfo;

uniform NormalDistributionFunctionCoefficients dCoefficients;
uniform GeometricAttenuationFunctionCoefficients gCoefficients;

uniform sampler2D irradianceMap;
uniform sampler2D preFilteredEnvironmentMap;
uniform sampler2D brdfIntegrationMap;

out vec4 FragColour;


#define PI 3.1415926535
#define EPSILON 0.000001


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

// ----- FRESNEL FUNCTION ---------------------------------------------------------------

/**
 * Approximate the Fresnel reflectivity of the material from this viewing
 * angle using Schlick's approximation.
 */
vec3 F(vec3 h, vec3 wo, vec3 F0, float roughness)
{
    float cosTheta = dot(h, wo);
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

// ----- THE BRDF -----------------------------------------------------------------------

/**
 * An implementation of the generalised Cook-Torrance BRDF. It relies
 * on the D, G and F functions.
 */
vec3 BRDF(vec3 p, vec3 wi, vec3 wo, vec3 n, vec3 F0)
{
    // Compute h (halfway) vector
    vec3 h = normalize(wi + wo);

    // Compute the three functions to determine the specular term
    float D_value = D(n, h, material.roughness);
    float G_value = G(n, wo, wi, material.roughness);
    vec3 F_value = F(h, wo, F0, material.roughness);

    // Use F and the material's metallic level to work out the diffuse coefficient
    vec3 kS = F_value;
    vec3 kD = (vec3(1.0) - kS) * (1 - material.metallic);

    // Compute the coefficient to be applied to the radiance from this light
    vec3 numerator = D_value * G_value * F_value;
    float denominator = 4 * max(dot(n, wo), 0.0) * max(dot(n, wi), 0.0);
    vec3 specular = numerator / max(denominator, 0.001);

    return (kD * material.albedo / PI) + specular;
}

// ----- COORDINATE TRANSFORMATIONS -----------------------------------------------------

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

// ----- COLOUR PROCESSING --------------------------------------------------------------

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

// ----- MAIN ---------------------------------------------------------------------------

void main()
{
    // Shorter names for the vectors
    vec3 n = Normal.xyz;
    vec3 p = Position_world.xyz;
    vec3 wo = normalize(cameraPosition - p);

    // Base reflectivity: we have to correct for the fact that this only really
    // makes sense for non-metals. For metals, we instead use the material's
    // albedo as F0.
    vec3 F0_corrected = mix(material.albedo, material.F0, material.metallic);

    // Add the contributions of all point light sources
    vec3 Lo = vec3(0.0);
    for (int i = 0; i < MAX_LIGHTS; i++) {

        // Read data about this light
        vec3 p_light = lightingInfo.lightPositions[i];
        vec3 luminance = lightingInfo.intensities[i] * lightingInfo.lightColours[i];

        // Work out how much energy we receive from this light source
        float attenuationAmount = 1 / pow(length(p - p_light), 2.0);  // Inverse-square law
        vec3 radiance = attenuationAmount * luminance;

        // Vector to the light
        vec3 wi = normalize(p_light - p);

        Lo += BRDF(p, wi, wo, n, F0_corrected) * radiance * max(dot(n, wi), 0.0);
    }

    // Add the contribution from the sun
    vec3 sunRadiance = sunInfo.colour * sunInfo.intensity;
    vec3 wiSun = sunInfo.direction;
    Lo += BRDF(p, wiSun, wo, n, F0_corrected) * sunRadiance * max(dot(n, wiSun), 0.0);

    // Work out kD coefficient
    vec3 F = F(n, wo, F0_corrected, material.roughness);
    vec3 kD = (1.0 - F) * (1.0 - material.metallic);

    // Add the diffuse contribution from the irradiance map
    vec2 uv = cubemapCoordsToUVs(n);
    vec4 sampledDiffuse = texture(irradianceMap, uv);
    Lo += kD * sampledDiffuse.rgb * material.albedo;

    // Compute the incoming specular light direction
    vec3 wi = 2.0 * dot(wo, n) * n - wo;

    // Sample the precomputed environment map and BRDF function
    float lod = material.roughness * 4.0;  // Remember the environment map encodes different roughnesses
                                           // in different mipmap levels
    vec3 environmentMapComponent = textureLod(preFilteredEnvironmentMap, cubemapCoordsToUVs(wi), lod).rgb;
    vec4 brdfScaleAndBias = texture(brdfIntegrationMap, vec2(max(dot(wo, n), 0.0), material.roughness));
    float F0_scale = brdfScaleAndBias.x;
    float F0_bias = brdfScaleAndBias.y;

    // Work out the specular contribution
    Lo += environmentMapComponent * (F0_scale * F + F0_bias);

    // Correct the output colour
    vec3 colour = toneMap(Lo);
    vec3 gammaEncoded = gammaEncode(colour);

    FragColour = vec4(gammaEncoded, 1.0f);
}
