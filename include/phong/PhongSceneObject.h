#ifndef PHYSICALLYBASEDRENDERER_PHONGSCENEOBJECT
#define PHYSICALLYBASEDRENDERER_PHONGSCENEOBJECT

#include <memory>
#include <vector>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "core/Camera.h"
#include "core/PointLightSource.h"
#include "core/ShaderProgram.h"
#include "core/Texture.h"

#include <optional>

namespace PBR {
namespace phong {

/**
 * Represents an object in a Phong scene.
 *
 * Consists of a list of vertices and information about the material.
 *
 * TODO: Support setting position and orientation of the object
 *
 * TODO: Handle shader programs better (there are only two possible ones we could
 *       need - one for textured, one for untextured)
 */
class PhongSceneObject {

private:
    std::shared_ptr<ShaderProgram> shaderProgram;

    // Material information
    float kD;
    float kS;
    float specularN;

    // Vertex data
    const float* vertices;
    const size_t vertexBufferLen;

    // Buffer IDs
    unsigned int vaoId;
    unsigned int vboId;

    // Texture information
    bool hasTexture;
    std::optional<std::shared_ptr<Texture>> texture;

public:
    /**
     * Create an untextured object.
     */
    PhongSceneObject(
            const float* vertices,
            size_t vertexBufferLen,
            std::shared_ptr<ShaderProgram> shaderProgram,
            float kD,
            float kS,
            float specularN = 1.0f);

    /**
     * Create a textured object.
     */
     PhongSceneObject(
             const float* vertices,
             size_t vertexBufferLen,
             std::shared_ptr<Texture> texture,
             std::shared_ptr<ShaderProgram> shaderProgram,
             float kD,
             float kS,
             float specularN = 1.0f);

    /**
     * Render this object.
     */
    void render(
            const Camera& camera,
            double time,
            glm::vec3 ambientLight,
            const std::vector<glm::vec3>& lightPositions,
            const std::vector<glm::vec3>& lightColours);

private:
    /**
     * Initialise the OpenGL buffers used by this object.
     */
    void initBuffers();

    /**
     * The model matrix for this object.
     */
    glm::mat4 getModelMatrix();

    /**
     * The stride length for this object's vertex buffer.
     */
    size_t vertexBufferStrideLength() const;
};

} // namespace phong
} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_PHONGSCENEOBJECT
