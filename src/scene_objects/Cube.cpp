#include "scene_objects/Cube.h"

#include <memory>
#include <vector>

#include "core/VertexData.h"

namespace {

const float CUBE_VERTICES_TEXTURED[] = {

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

const float CUBE_VERTICES_UNTEXTURED[] = {

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

const unsigned int INDICES[] = {

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

} // anonymous namespace

namespace PBR::scene_objects {

std::shared_ptr<VertexData> Cube::s_texturedVertexData;
std::shared_ptr<VertexData> Cube::s_untexturedVertexData;

std::shared_ptr<VertexData> Cube::getTexturedVertexData()
{
    if (!s_texturedVertexData) {
        std::shared_ptr<std::vector<float>> vertexData(
                new std::vector<float>(
                        CUBE_VERTICES_TEXTURED, CUBE_VERTICES_TEXTURED + sizeof(CUBE_VERTICES_TEXTURED)));
        std::shared_ptr<std::vector<unsigned int>> indexData(
                new std::vector<unsigned int>(INDICES, INDICES + sizeof(INDICES)));
        s_texturedVertexData = std::make_shared<VertexData>(vertexData, indexData, true);
    }
    return s_texturedVertexData;
}

std::shared_ptr<VertexData> Cube::getUntexturedVertexData()
{
    if (!s_untexturedVertexData) {
        std::shared_ptr<std::vector<float>> vertexData(
                new std::vector<float>(
                        CUBE_VERTICES_UNTEXTURED, CUBE_VERTICES_UNTEXTURED + sizeof(CUBE_VERTICES_UNTEXTURED)));
        std::shared_ptr<std::vector<unsigned int>> indexData(
                new std::vector<unsigned int>(INDICES, INDICES + sizeof(INDICES)));
        s_untexturedVertexData = std::make_shared<VertexData>(vertexData, indexData, false);
    }
    return s_untexturedVertexData;
}

Cube::Cube(glm::vec3 pos,
           glm::vec3 orientation,
           float scale,
           Material material,
           const std::shared_ptr<Texture>& texture)
        :SceneObject(pos, orientation, glm::vec3(scale), material, getTexturedVertexData(), texture)
{
}

Cube::Cube(glm::vec3 pos,
           glm::vec3 orientation,
           float scale,
           Material material,
           glm::vec3 colour)
        :SceneObject(pos, orientation, glm::vec3(scale), material, getUntexturedVertexData(), colour)
{
}

} // namespace PBR::scene_objects
