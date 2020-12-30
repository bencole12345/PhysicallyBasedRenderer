#ifndef PHYSICALLYBASEDRENDERER_SCENEOBJECT
#define PHYSICALLYBASEDRENDERER_SCENEOBJECT

#include <memory>
#include <optional>
#include <vector>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "core/Material.h"
#include "core/Texture.h"
#include "core/VertexData.h"

namespace PBR {

/**
 * Includes all information about a renderer-independent scene object.
 */
struct SceneObject {

    // Positional information
    glm::vec3 pos;
    glm::vec3 orientation;
    glm::vec3 scale;

    // Vertex data
    std::shared_ptr<VertexData> vertexData;

    // Material information
    Material material;

    // Texture/Colour
    std::optional<std::shared_ptr<Texture>> texture;
    std::optional<glm::vec3> colour;

    /**
     * Create a textured scene object.
     */
    SceneObject(glm::vec3 pos, glm::vec3 orientation, glm::vec3 scale, Material material,
            std::shared_ptr<VertexData> vertexData, const std::shared_ptr<Texture>& texture);

    /**
     * Create an untextured scene object.
     */
    SceneObject(glm::vec3 pos, glm::vec3 orientation, glm::vec3 scale, Material material,
            std::shared_ptr<VertexData> vertexData, glm::vec3 colour);

    /**
     * Computes the model matrix for this scene object.
     */
    glm::mat4 getModelMatrix() const;

    /**
     * Computes the rotation matrix corresponding to this scene object's orientation.
     */
    glm::mat4 getRotationMatrix() const;

    /**
     * @return `true` if this scene object is textured, `false` if it is untextured
     */
    bool hasTexture() const;

    /**
     * @return `true` if this scene object has a constant colour, `false` otherwise
     */
    bool hasConstantColour() const;
};

} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_SCENEOBJECT
