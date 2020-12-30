#include "scene_objects/Plane.h"

#include <memory>

#include <glm/vec3.hpp>

#include "core/VertexData.h"

namespace {

const float PLANE_VERTICES_TEXTURED[] = {

        // Position(x, y, z), Normal(x, y, z), TexCoord(u, v)

        // Top
        0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,

        // Bottom
        -0.5f, 0.0f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.0f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.0f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
};

const float PLANE_VERTICES_UNTEXTURED[] = {

        // Position(x, y, z), Normal(x, y, z)

        // Top
        0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f,

        // Bottom
        -0.5f, 0.0f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, 0.0f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, 0.0f, -0.5f, 0.0f, -1.0f, 0.0f,
};

} // anonymous namespace

namespace PBR::scene_objects {

std::shared_ptr<VertexData> Plane::s_texturedVertexData;
std::shared_ptr<VertexData> Plane::s_untexturedVertexData;

std::shared_ptr<VertexData> Plane::getTexturedVertexData()
{
    if (!s_texturedVertexData) {
        s_texturedVertexData = std::make_shared<VertexData>(PLANE_VERTICES_TEXTURED, sizeof(PLANE_VERTICES_TEXTURED),
                                                            true);
    }
    return s_texturedVertexData;
}

std::shared_ptr<VertexData> Plane::getUntexturedVertexData()
{
    if (!s_untexturedVertexData) {
        s_untexturedVertexData = std::make_shared<VertexData>(PLANE_VERTICES_UNTEXTURED,
                                                              sizeof(PLANE_VERTICES_UNTEXTURED), false);
    }
    return s_untexturedVertexData;
}

Plane::Plane(glm::vec3 pos, glm::vec3 orientation, glm::vec2 dimensions, Material material,
             const std::shared_ptr<Texture>& texture)
        :SceneObject(pos, orientation, glm::vec3(dimensions.x, 1.0f, dimensions.y), material, getTexturedVertexData(),
                     texture)
{
}

Plane::Plane(glm::vec3 pos, glm::vec3 orientation, glm::vec2 dimensions, Material material, glm::vec3 colour)
        :SceneObject(pos, orientation, glm::vec3(dimensions.x, 1.0f, dimensions.y), material, getUntexturedVertexData(),
                     colour)
{
}

} // namespace PBR::scene_objects
