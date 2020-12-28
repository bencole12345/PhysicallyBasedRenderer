#version 410

// TODO: Find a way to embed this in the binary

in vec3 TexCoords;

uniform samplerCube skybox;

out vec4 FragColour;

void main()
{
    FragColour = texture(skybox, TexCoords);
}
