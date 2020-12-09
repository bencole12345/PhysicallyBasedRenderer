#version 410 core

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

in vec3 MaterialColour;
in vec4 Position_world;
in vec4 Normal;

uniform double time;
uniform vec4 cameraPosition;
uniform Material material;
uniform vec4 ambientLight;
uniform PointLightSources lights;

out vec4 FragColor;

void main()
{
   vec4 colour = vec4(0.0f, 0.0f, 0.0f, 1.0f);

   // Ambient
   colour += vec4(MaterialColour, 1.0f) * ambientLight;

   // Diffuse
   for (int i = 0; i < MAX_LIGHTS; i++) {
      vec4 lightColour = vec4(lights.colours[i], 1.0f);
      vec3 toLight = normalize(lights.positions[i] - Position_world.xyz);
      float dotProduct = max(dot(toLight, Normal.xyz), 0.0f);
      colour += material.kD * dotProduct * vec4(MaterialColour, 1.0f) * lightColour;
   }

   // Specular
   for (int i = 0; i < MAX_LIGHTS; i++) {
      vec4 lightColour = vec4(lights.colours[i], 1.0f);
      vec3 toLight = normalize(lights.positions[i] - Position_world.xyz);
      vec3 reflected = -2.0f * toLight + dot(toLight, Normal.xyz) * Normal.xyz;
      vec3 toEye = cameraPosition.xyz - Position_world.xyz;
      float dotProduct = max(dot(reflected, normalize(toEye)), 0.0f);
      float coefficient = pow(dotProduct, material.specularN);
      colour += material.kS * coefficient * vec4(MaterialColour, 1.0f) * lightColour;
   }

   FragColor = clamp(colour, 0.0f, 1.0f);
}
