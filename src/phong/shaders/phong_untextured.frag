#version 410 core

// TODO: Find a way to embed this into the binary

#define MAX_LIGHTS 16

struct Material {
   float kD;
   float kS;
   float specularN;
};

struct PointLightSources {
   vec3 positions[MAX_LIGHTS];
   vec3 colours[MAX_LIGHTS];
};

struct LightingInfo {
   vec3 ambientLight;
   vec3 pointLightPositions[MAX_LIGHTS];
   vec3 pointLightColours[MAX_LIGHTS];
};

in vec4 Position_world;
in vec4 Normal;

uniform vec3 cameraPosition;
uniform Material material;
uniform LightingInfo lightingInfo;
uniform vec3 surfaceColour;

out vec4 FragColor;

void main()
{
   vec4 colour = vec4(0.0f, 0.0f, 0.0f, 1.0f);

   // Ambient
   colour += vec4(surfaceColour * lightingInfo.ambientLight, 1.0f);

   // Diffuse
   for (int i = 0; i < MAX_LIGHTS; i++) {
      vec4 lightColour = vec4(lightingInfo.pointLightColours[i], 1.0f);
      vec3 toLight = normalize(lightingInfo.pointLightPositions[i] - Position_world.xyz);
      float dotProduct = max(dot(toLight, Normal.xyz), 0.0f);
      colour += material.kD * dotProduct * vec4(surfaceColour, 1.0f) * lightColour;
   }

   // Specular
   for (int i = 0; i < MAX_LIGHTS; i++) {
      vec4 lightColour = vec4(lightingInfo.pointLightColours[i], 1.0f);
      vec3 toLight = normalize(lightingInfo.pointLightPositions[i] - Position_world.xyz);
      vec3 reflected = -2.0f * toLight + dot(toLight, Normal.xyz) * Normal.xyz;
      vec3 toEye = cameraPosition - Position_world.xyz;
      float dotProduct = max(dot(reflected, normalize(toEye)), 0.0f);
      float coefficient = pow(dotProduct, material.specularN);
      colour += material.kS * coefficient * vec4(surfaceColour, 1.0f) * lightColour;
   }

   FragColor = clamp(colour, 0.0f, 1.0f);
}
