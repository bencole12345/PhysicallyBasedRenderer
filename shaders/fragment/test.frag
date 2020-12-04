#version 410 core

uniform double time;

out vec4 FragColor;

void main()
{
   float red_amount = 0.5 + 0.5 * sin(float(time));
   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
