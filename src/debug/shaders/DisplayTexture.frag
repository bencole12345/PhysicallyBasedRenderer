#version 410

in vec2 TexCoords;

uniform sampler2D textureToDisplay;
uniform bool isHDR;
uniform bool useMipmapSampling;
uniform float lod;

out vec4 FragColour;

vec3 toneMap(vec3 colourHDR)
{
    return colourHDR / (vec3(1.0) + colourHDR);
}

vec3 gammaEncode(vec3 colour)
{
    return pow(colour, vec3(1.0 / 2.2));
}

void main()
{
    vec4 value;
    if (useMipmapSampling) {
        value = textureLod(textureToDisplay, TexCoords, lod);
    }
    else {
        value = texture(textureToDisplay, TexCoords);
    }

    if (isHDR) {
        value.rgb = gammaEncode(toneMap(value.rgb));
    }

    FragColour = value;
}
