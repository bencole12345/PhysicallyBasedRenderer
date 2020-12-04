#ifndef PHYSICALLYBASEDRENDERER_SHADERPROGRAM
#define PHYSICALLYBASEDRENDERER_SHADERPROGRAM

#include <vector>

#include "Shader.h"

namespace PBR {

class ShaderProgram {
private:
    const unsigned int shaderProgramId;
    const Shader& vertexShader;
    const Shader& fragmentShader;
public:
    ShaderProgram(Shader& vertexShader, Shader& fragmentShader);
    ~ShaderProgram();

    unsigned int id() const;
};

} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_SHADERPROGRAM
