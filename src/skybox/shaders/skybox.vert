#version 410

// TODO: Find a way to embed this in the binary

layout (location = 0) in vec3 VertexPos;

uniform mat4 Projection;
uniform mat4 View;

out vec3 TexCoords;

void main()
{
    gl_Position = Projection * View * vec4(VertexPos, 1.0);

    // Make sure this doesn't render on top of anything
    // 
    // Explanation: The distance from the camera is calculated as z / w
    // when converting from homogeneous coordinates to screen space
    // coordinates. By setting this, we force this to be exactly 1, the
    // maximum possible distance, meaning that the skybox will only ever
    // be rendered if no geometry has been rendered to that fragment.
    gl_Position.z = gl_Position.w;

    TexCoords = VertexPos;
}
