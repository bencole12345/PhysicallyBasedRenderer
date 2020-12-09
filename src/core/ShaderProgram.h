#ifndef PHYSICALLYBASEDRENDERER_SHADERPROGRAM
#define PHYSICALLYBASEDRENDERER_SHADERPROGRAM

#include <vector>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "Shader.h"

namespace PBR {

class ShaderProgram {
private:
    const unsigned int shaderProgramId;

    std::shared_ptr<Shader> vertexShader;
    std::shared_ptr<Shader> fragmentShader;

public:
    ShaderProgram(const std::shared_ptr<Shader>& vertexShader, const std::shared_ptr<Shader>& fragmentShader);
    ~ShaderProgram();

    unsigned int id() const;

    void setUniform(const std::string& name, float value);
    void setUniform(const std::string& name, double value);
    void setUniform(const std::string& name, glm::vec3 value);
    void setUniform(const std::string& name, glm::vec4 value);
    void setUniform(const std::string& name, const glm::mat4& matrix);
    void setUniform(const std::string& name, std::vector<glm::vec3> values);
};

} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_SHADERPROGRAM
