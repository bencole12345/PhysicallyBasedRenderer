#ifndef PHYSICALLYBASEDRENDERER_PHONGSCENEOBJECT
#define PHYSICALLYBASEDRENDERER_PHONGSCENEOBJECT

#include <memory>
#include <vector>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "core/ShaderProgram.h"
#include "core/Camera.h"
#include "core/PointLightSource.h"

namespace PBR {
namespace phong {

/**
 * Represents an object in a Phong scene.
 *
 * Consists of a list of vertices and information about the material.
 *
 * TODO: Support setting position and orientation of the object
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

public:
    PhongSceneObject(
            const float* vertices,
            size_t vertexBufferLen,
            std::shared_ptr<ShaderProgram> shaderProgram,
            float kD,
            float kS,
            float specularN = 1.0f);

    void render(
            const Camera& camera,
            double time,
            glm::vec3 ambientLight,
            const std::vector<glm::vec3>& lightPositions,
            const std::vector<glm::vec3>& lightColours);

private:
    glm::mat4 getModelMatrix();
};

} // namespace phong
} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_PHONGSCENEOBJECT
