#ifndef PHYSICALLYBASEDRENDERER_SCENEOBJECT
#define PHYSICALLYBASEDRENDERER_SCENEOBJECT

#include <glm/mat4x4.hpp>

#include "ShaderProgram.h"

namespace PBR {

class SceneObject {
private:
    const ShaderProgram& shaderProgram;

    // Vertex data
    const float *vertices;
    const size_t verticesCount;

    // Buffer IDs
    unsigned int vaoId;
    unsigned int vboId;

public:
    SceneObject(const float *vertices, size_t verticesCount, const ShaderProgram& shaderProgram);

    void render(glm::mat4x4 mvpMatrix) const;
};

} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_SCENEOBJECT
