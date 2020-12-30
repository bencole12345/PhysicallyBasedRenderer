#include "core/VertexData.h"

#include <vector>

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

#define POSITION_OFFSET 0
#define NORMAL_OFFSET 3
#define TEXTURE_COORDS_OFFSET 6

namespace {

size_t getStride(bool textured)
{
    // We always have position (3) and normals (3). There are two more
    // elements for texture coordinates if the data includes them.
    return textured ? 8 * sizeof(float) : 6 * sizeof(float);
}

} // anonymous namespace

namespace PBR {

VertexData::VertexData(const float *data, size_t bufferLength, bool textured)
    : data(data, data + bufferLength),
      stride(getStride(textured)),
      vaoId(),
      vboId(),
      hasNormals(true),
      hasTextureCoordinates(textured),
      positionsOffset(POSITION_OFFSET),
      normalsOffset(NORMAL_OFFSET),
      textureCoordinatesOffset(TEXTURE_COORDS_OFFSET)
{
    // Create the vertex array
    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);

    // Create the vertex buffer
    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, bufferLength, data, GL_STATIC_DRAW);

    // Vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*) (POSITION_OFFSET * sizeof(float)));
    glEnableVertexAttribArray(0);

    // Normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*) (NORMAL_OFFSET * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture coordinates
    if (textured) {
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*) (TEXTURE_COORDS_OFFSET * sizeof(float)));
        glEnableVertexAttribArray(2);
    }
}

VertexData::~VertexData()
{
    glDeleteBuffers(1, &vboId);
    glDeleteVertexArrays(1, &vaoId);
}

} // namespace PBR
