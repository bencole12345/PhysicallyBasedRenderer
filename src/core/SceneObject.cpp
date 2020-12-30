#include "core/SceneObject.h"

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <utility>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/VertexData.h"

namespace PBR {

SceneObject::SceneObject(glm::vec3 pos, glm::vec3 orientation, glm::vec3 scale, Material material,
        std::shared_ptr<VertexData> vertexData, const std::shared_ptr<Texture>& texture)
        :pos(pos),
         orientation(orientation),
         scale(scale),
         material(material),
         vertexData(std::move(vertexData)),
         texture(texture)
{
}

SceneObject::SceneObject(glm::vec3 pos, glm::vec3 orientation, glm::vec3 scale, Material material,
        std::shared_ptr<VertexData> vertexData, glm::vec3 colour)
        :pos(pos),
         orientation(orientation),
         scale(scale),
         material(material),
         vertexData(std::move(vertexData)),
         colour(colour)
{
}

glm::mat4 SceneObject::getModelMatrix() const
{
    // Remember that these are matrices, so we apply them in reverse order.
    // We start with an identity matrix and apply the transformations to it
    // in last-to-first order.
    glm::mat4 identity(1.0f);

    // Last do the translation
    auto translated = glm::translate(identity, pos);

    // Before that, apply the scaling
    auto scaled = glm::scale(translated, scale);

    // The first step is to perform the rotations.
    auto rotatedZ = glm::rotate(scaled, orientation[2], glm::vec3(0.0f, 0.0f, 1.0f));
    auto rotatedYZ = glm::rotate(rotatedZ, orientation[1], glm::vec3(0.0f, 1.0f, 0.0f));
    auto rotatedXYZ = glm::rotate(rotatedYZ, orientation[0], glm::vec3(1.0f, 0.0f, 0.0f));

    return rotatedXYZ;
}

glm::mat4 SceneObject::getRotationMatrix() const
{
    glm::mat4 identity(1.0f);
    auto rotatedZ = glm::rotate(identity, orientation[2], glm::vec3(0.0f, 0.0f, 1.0f));
    auto rotatedYZ = glm::rotate(rotatedZ, orientation[1], glm::vec3(0.0f, 1.0f, 0.0f));
    auto rotatedXYZ = glm::rotate(rotatedYZ, orientation[0], glm::vec3(1.0f, 0.0f, 0.0f));
    return rotatedXYZ;
}

bool SceneObject::hasTexture() const
{
    return texture.has_value();
}

bool SceneObject::hasConstantColour() const
{
    return colour.has_value();
}

} // namespace PBR
