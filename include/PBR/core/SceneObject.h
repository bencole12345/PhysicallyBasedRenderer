#ifndef PHYSICALLYBASEDRENDERER_SCENEOBJECT
#define PHYSICALLYBASEDRENDERER_SCENEOBJECT

#include <memory>
#include <optional>
#include <utility>
#include <vector>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/Texture.h"
#include "core/VertexData.h"

namespace PBR {

/**
 * Includes all information about a renderer-independent scene object.
 */
template<class MaterialType>
struct SceneObject {

    // Positional information
    glm::vec3 pos;
    glm::vec3 orientation;
    glm::vec3 scale;

    // Vertex data
    std::shared_ptr<VertexData> vertexData;

    // Material information
    MaterialType material;

    // Texture
    std::optional<std::shared_ptr<Texture>> texture;

    /**
     * Construct a scene object.
     *
     * @param pos The object's position in world space
     * @param orientation The orientation of the object in world space
     * @param scale The object's scale factor along the x, y and z axes in model space
     * @param material The object's material
     * @param texture (Optional) The object's texture
     */
    SceneObject(glm::vec3 pos, glm::vec3 orientation, glm::vec3 scale, MaterialType material,
                std::shared_ptr<VertexData> vertexData,
                std::optional<std::shared_ptr<Texture>> texture = std::nullopt);

    /**
     * Computes the model matrix for this scene object.
     */
    glm::mat4 getModelMatrix() const;

    /**
     * Computes the rotation matrix for this scene object.
     * 
     * This is equivalent to applying the model matrix, but without the scale
     * and translation operations.
     */
    glm::mat4 getRotationMatrix() const;

    /**
     * @return `true` if this scene object is textured, `false` if it is untextured
     */
    bool hasTexture() const;

};

template<class MaterialType>
SceneObject<MaterialType>::SceneObject(glm::vec3 pos, glm::vec3 orientation, glm::vec3 scale, MaterialType material,
                                       std::shared_ptr<VertexData> vertexData,
                                       std::optional<std::shared_ptr<Texture>> texture)
        :pos(pos),
         orientation(orientation),
         scale(scale),
         material(material),
         vertexData(std::move(vertexData)),
         texture(std::move(texture))
{
}

template<class MaterialType>
glm::mat4 SceneObject<MaterialType>::getModelMatrix() const
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

template<class MaterialType>
glm::mat4 SceneObject<MaterialType>::getRotationMatrix() const
{
    glm::mat4 identity(1.0f);

    auto rotatedZ = glm::rotate(identity, orientation[2], glm::vec3(0.0f, 0.0f, 1.0f));
    auto rotatedYZ = glm::rotate(rotatedZ, orientation[1], glm::vec3(0.0f, 1.0f, 0.0f));
    auto rotatedXYZ = glm::rotate(rotatedYZ, orientation[0], glm::vec3(1.0f, 0.0f, 0.0f));

    return rotatedXYZ;
}

template<class MaterialType>
bool SceneObject<MaterialType>::hasTexture() const
{
    return texture.has_value();
}

} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_SCENEOBJECT
