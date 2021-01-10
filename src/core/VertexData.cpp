#include "core/VertexData.h"

#include <memory>
#include <vector>

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define POSITION_OFFSET 0
#define NORMAL_OFFSET 3
#define TEXTURE_COORDS_OFFSET 6

namespace {

inline
size_t getStride(bool textured)
{
    // We always have position (3) and normals (3). There are two more
    // elements for texture coordinates if the data includes them.
    return textured ? 8 * sizeof(float) : 6 * sizeof(float);
}

} // anonymous namespace

namespace PBR {

VertexData::VertexData(std::shared_ptr<std::vector<float>> vertexData, std::shared_ptr<std::vector<unsigned int>> elementData, bool textured)
        :data(std::move(vertexData)),
         indices(std::move(elementData)),
         stride(getStride(textured)),
         vaoId(),
         vboId(),
         eboId(),
         hasNormals(true),
         hasTextureCoordinates(textured),
         positionOffset(POSITION_OFFSET),
         normalsOffset(NORMAL_OFFSET),
         textureCoordinatesOffset(TEXTURE_COORDS_OFFSET)
{
    initBuffers();
}

VertexData::~VertexData()
{
    glDeleteBuffers(1, &vboId);
    glDeleteBuffers(1, &eboId);
    glDeleteVertexArrays(1, &vaoId);
}

void VertexData::initBuffers()
{
    // Create the vertex array object
    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);

    // Create the vertex buffer to hold the actual vertex data
    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, data->size() * sizeof(float), &(*data)[0], GL_STATIC_DRAW);

    // Create the element buffer object to hold the indices
    glGenBuffers(1, &eboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(float), &(*indices)[0], GL_STATIC_DRAW);

    // Vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*) (POSITION_OFFSET * sizeof(float)));
    glEnableVertexAttribArray(0);

    // Normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*) (NORMAL_OFFSET * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture coordinates
    if (hasTextureCoordinates) {
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*) (TEXTURE_COORDS_OFFSET * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

} // namespace PBR
