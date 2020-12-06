#version 410 core

struct Material {
   float kD;
   float kS;
   float specularN;
};

struct Light {
   vec3 position;
   vec3 colour;
};

in vec3 MaterialColour;
in vec4 Position_world;
in vec4 Normal;

uniform double time;
uniform vec4 cameraPosition;
uniform Material material;
uniform vec4 ambientLight;
uniform Light light;

out vec4 FragColor;

void main()
{
   vec4 colour = vec4(0.0f, 0.0f, 0.0f, 1.0f);

   // Ambient
   colour += vec4(MaterialColour, 1.0f) * ambientLight;

   // Diffuse
   vec4 lightColour = vec4(light.colour, 1.0f);
   vec3 toLight = normalize(light.position - Position_world.xyz);
   float dotProduct = max(dot(toLight, Normal.xyz), 0.0f);
   colour += material.kD * dotProduct * vec4(MaterialColour, 1.0f) * lightColour;

   // Specular
   vec3 reflected = -2.0f * toLight + dot(toLight, Normal.xyz) * Normal.xyz;
   vec3 toEye = cameraPosition.xyz - Position_world.xyz;
   dotProduct = max(dot(reflected, normalize(toEye)), 0.0f);
   float coefficient = pow(dotProduct, material.specularN);
   colour += material.kS * coefficient * vec4(MaterialColour, 1.0f) * lightColour;

   FragColor = clamp(colour, 0.0f, 1.0f);
}
