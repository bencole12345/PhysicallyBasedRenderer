#version 410

layout (location = 0) in vec3 VertexPos;
layout (location = 1) in vec3 Normal_modelCoords;
layout (location = 2) in vec3 materialBaseColour;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform double time;

out vec4 Normal;
out vec4 Position_world;
out vec3 MaterialColour;

void main()
{
   // Convert to homogeneous coordinates
   vec4 model_coords = vec4(VertexPos, 1.0);
   vec4 normal_coords = vec4(Normal_modelCoords, 1.0);

   // Compute the world position of this vertex
   Position_world = Model * model_coords;

   // Compute the projected onscreen position of this vertex
   gl_Position = Projection * View * Model * model_coords;

   // Rotate the normal of this vertex into world space
   Normal = Model * normal_coords;

   // Pass through the material colour
   MaterialColour = materialBaseColour;
}
