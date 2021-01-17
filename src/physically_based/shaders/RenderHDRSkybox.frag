#version 410

in vec3 TexCoords;

uniform sampler2D SkyboxTexture;

out vec4 FragColour;


#define PI 3.14159


/**
 * Convert from cubemap coordinates to UV coordiantes for the texture.
 */
vec2 cubemapCoordsToUVs(vec3 cubemapCoords)
{
    // Radius when projected into the XZ plane
    float r = sqrt(TexCoords.x * TexCoords.x + TexCoords.z * TexCoords.z);

    // Rotation clockwise from -Z axis
    float phi = atan(TexCoords.x, -TexCoords.z);

    // Rotation up from XZ plane
    float theta = atan(TexCoords.y, r);

    // Corresponding texture location
    float u = 0.5 + phi / (2 * PI);
    float v = 0.5 + theta / (2 * PI);

    return vec2(u, v);
}

/**
 * Apply tone mapping to a HDR colour to make it representible in regular
 * colours.
 */
vec3 toneMap(vec3 colourHDR)
{
    return colourHDR / (vec3(1.0) + colourHDR);
}

/**
 * Gamma encode a colour in linear colour space.
 */
vec3 gammaEncode(vec3 colour)
{
    return pow(colour, vec3(1.0 / 2.2));
}

void main()
{
    // Perform the coordinate transform
    vec2 uv = cubemapCoordsToUVs(TexCoords);

    // Sample the texture
    vec3 sampled = texture(SkyboxTexture, uv).rgb;

    // Correct colours and output
    vec3 toneMapped = toneMap(sampled);
    vec3 gammaEncoded = gammaEncode(toneMapped);
    FragColour = vec4(gammaEncoded, 1.0);
}
