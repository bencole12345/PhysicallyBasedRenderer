#version 410

layout (location = 0) in vec3 pos;

uniform double time;
uniform mat4x4 MVP;

void main()
{
   float x_coord = pos.x + 0.2 * sin(float(time));
   float y_coord = pos.y + 0.15 * sin(2.1 * float(time));
   vec4 model_coords = vec4(pos.x, pos.y, pos.z, 1.0);
   gl_Position = MVP * model_coords;
}
