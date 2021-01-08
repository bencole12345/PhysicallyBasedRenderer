#version 410 core

// TODO: Find a way to embed this into the binary

#define MAX_LIGHTS 16

struct PhysicallyBasedMaterial {
    vec3 albedo;
    float roughness;
    float metallic;
};

struct PointLightSources {
    vec3 positions[MAX_LIGHTS];
    vec3 colours[MAX_LIGHTS];
};

struct LightingInfo {
//    vec3 ambientLight;
    vec3 pointLightPositions[MAX_LIGHTS];
    vec3 pointLightColours[MAX_LIGHTS];
};

in vec4 Normal;
in vec4 Position_world;
in vec2 TexCoord;

uniform vec3 cameraPosition;
uniform PhysicallyBasedMaterial material;
uniform LightingInfo lightingInfo;
//uniform sampler2D surfaceTexture;
uniform samplerCube skybox;

out vec4 FragColor;

float attenuation(vec3 from, vec3 to)
{
    float dist = length(to - from);
    return 1 / (dist * dist);
}

float alphaFromRoughness(float roughness)
{
    // TODO: Work out what the actual mapping is supposed to be here
    return roughness;
}

float K_direct(float alpha)
{
    return (alpha + 1.0) * (alpha + 1.0) / 8.0;
}

float K_imageBased(float alpha)
{
    return alpha * alpha / 2.0;
}

float G_SchlickGGX(vec3 n, vec3 v, float k)
{
    return dot(n, v) / (dot(n, v) * (1 - k) + k);
}

float G(vec3 n, vec3 v, vec3 l, float k)
{
    return G_SchlickGGX(n, v, k) * G_SchlickGGX(n, l, k);
}

void main()
{
    vec4 Lo = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    vec4 baseColour = vec4(material.albedo, 1.0f);

    vec3 N = Normal.xyz;
    vec3 Pos = Position_world.xyz;
    vec3 V = normalize(cameraPosition - Pos);

    float alpha = alphaFromRoughness(material.roughness);
    float k = K_direct(alpha);

    // Integrate by summing over all light sources
    for (int i = 0; i < MAX_LIGHTS; i++) {

        vec3 lightPos = lightingInfo.pointLightPositions[i];
        vec4 sourceLuminance = vec4(lightingInfo.pointLightColours[i], 1.0f);
        vec4 luminance = attenuation(Pos, lightPos) * sourceLuminance;

        // Vector to the light
        vec3 l = normalize(lightPos - Pos);

        // Halfway vector
        vec3 h = normalize(V + l);


        Lo += G(N, V, l, k) * luminance;
    }

    FragColor = clamp(Lo, 0.0f, 1.0f);
}
