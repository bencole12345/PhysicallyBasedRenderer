#version 410

// DISCLAIMER: This file is just a modified version of how they did it
// here: https://learnopengl.com/Advanced-OpenGL/Framebuffers

layout (location = 0) in vec2 PositionNDC;
layout (location = 1) in vec2 TexCoords_in;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(PositionNDC.x, PositionNDC.y, 0.0, 1.0);
    TexCoords = TexCoords_in;
}
