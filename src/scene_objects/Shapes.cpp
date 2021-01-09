#include "scene_objects/Shapes.h"

#include <memory>
#include <vector>

#include "core/VertexData.h"

namespace {

constexpr float CUBE_VERTICES_TEXTURED[] = {

        // Position(x, y, z), Normal(x, y, z), TexCoord(u, v)

        // Back face
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,

        // Front face
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,

        // Left face
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

        // Right face
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

        // Top face
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,

        // Bottom face
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
};

constexpr float CUBE_VERTICES_UNTEXTURED[] = {

        // Position(x, y, z), Normal(x, y, z)

        // Back face
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

        // Front face
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        // Left face
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

        // Right face
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,

        // Top face
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,

        // Bottom face
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
};

constexpr unsigned int CUBE_INDICES[] = {

        // Back face
        0, 2, 1, 1, 2, 3,

        // Front face
        4, 6, 5, 5, 6, 7,

        // Left face
        8, 10, 9, 9, 10, 11,

        // Right face
        12, 14, 13, 13, 14, 15,

        // Top face
        16, 18, 17, 17, 18, 19,

        // Bottom face
        20, 22, 21, 21, 22, 23,
};

constexpr float PLANE_VERTICES_TEXTURED[] = {

        // Position(x, y, z), Normal(x, y, z), TexCoord(u, v)

        // Top
        -0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,

        // Bottom
        -0.5f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.0f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.0f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
};

constexpr float PLANE_VERTICES_UNTEXTURED[] = {

        // Position(x, y, z), Normal(x, y, z)

        // Top
        -0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f,

        // Bottom
        -0.5f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, 0.0f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, 0.0f, -0.5f, 0.0f, -1.0f, 0.0f,
};

constexpr unsigned int PLANE_INDICES[] = {

        // Top
        0, 2, 1, 1, 2, 3,

        // Bottom
        4, 6, 5, 5, 6, 7
};

} // anonymous namespace

namespace PBR::scene_objects {

// Initialise static objects to avoid a linker error
std::shared_ptr<VertexData> Shapes::s_texturedCubeVertexData;
std::shared_ptr<VertexData> Shapes::s_untexturedCubeVertexData;
std::shared_ptr<VertexData> Shapes::s_texturedPlaneVertexData;
std::shared_ptr<VertexData> Shapes::s_untexturedPlaneVertexData;

std::shared_ptr<VertexData> Shapes::texturedCube()
{
    bool textured = true;
    ensureVertexDataInitialised(s_texturedCubeVertexData, CUBE_VERTICES_TEXTURED,
                                sizeof(CUBE_VERTICES_TEXTURED) / sizeof(float), CUBE_INDICES,
                                sizeof(CUBE_INDICES) / sizeof(unsigned int), textured);
    return s_texturedCubeVertexData;
}

std::shared_ptr<VertexData> Shapes::untexturedCube()
{
    bool textured = false;
    ensureVertexDataInitialised(s_untexturedCubeVertexData, CUBE_VERTICES_UNTEXTURED,
                                sizeof(CUBE_VERTICES_UNTEXTURED) / sizeof(float), CUBE_INDICES,
                                sizeof(CUBE_INDICES) / sizeof(unsigned int), textured);
    return s_untexturedCubeVertexData;
}

std::shared_ptr<VertexData> Shapes::texturedPlane()
{
    bool textured = true;
    ensureVertexDataInitialised(s_texturedPlaneVertexData, PLANE_VERTICES_TEXTURED,
                                sizeof(PLANE_VERTICES_TEXTURED) / sizeof(float), PLANE_INDICES,
                                sizeof(PLANE_INDICES) / sizeof(unsigned int), textured);
    return s_texturedPlaneVertexData;
}

std::shared_ptr<VertexData> Shapes::untexturedPlane()
{
    bool textured = false;
    ensureVertexDataInitialised(s_untexturedPlaneVertexData, PLANE_VERTICES_UNTEXTURED,
                                sizeof(PLANE_VERTICES_UNTEXTURED) / sizeof(float), PLANE_INDICES,
                                sizeof(PLANE_INDICES) / sizeof(unsigned int), textured);
    return s_untexturedPlaneVertexData;
}

void Shapes::ensureVertexDataInitialised(std::shared_ptr<VertexData>& vertexData, const float vertexBuffer[],
                                         size_t vertexBufferLen, const unsigned int indexBuffer[],
                                         size_t indexBufferLen,
                                         bool textured)
{
    if (!vertexData) {
        std::shared_ptr<std::vector<float>> createdVertexBuffer(
                new std::vector<float>(vertexBuffer, vertexBuffer + vertexBufferLen));
        std::shared_ptr<std::vector<unsigned int>> createdIndexBuffer(
                new std::vector<unsigned int>(indexBuffer, indexBuffer + indexBufferLen));
        vertexData = std::make_shared<VertexData>(createdVertexBuffer, createdIndexBuffer, textured);
    }
}

} // namespace PBR::scene_objects
