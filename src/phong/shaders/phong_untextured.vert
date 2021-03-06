#version 410

layout (location = 0) in vec3 VertexPos;
layout (location = 1) in vec3 Normal_modelCoords;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 NormalsRotation;

out vec4 Normal;
out vec4 Position_world;

void main()
{
   // Convert to homogeneous coordinates
   vec4 model_coords = vec4(VertexPos, 1.0);
   vec4 normal_coords = vec4(Normal_modelCoords, 1.0);

   // Compute the world position of this vertex
   Position_world = NormalsRotation * model_coords;

   // Compute the projected onscreen position of this vertex
   gl_Position = Projection * View * Model * model_coords;

   // Rotate the normal of this vertex into world space
   Normal = Model * normal_coords;
}
