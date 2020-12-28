#ifndef PHYSICALLYBASEDRENDERER_SHADERPROGRAM
#define PHYSICALLYBASEDRENDERER_SHADERPROGRAM

#include <string_view>
#include <vector>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace PBR {

/**
 * Wraps a shader program containing a vertex and fragment shader.
 */
class ShaderProgram {
private:
    const unsigned int shaderProgramId;

public:
    ShaderProgram(std::string_view vertexShaderLocation, std::string_view fragmentShaderLocation);
    ~ShaderProgram();

    unsigned int id() const;

    void setUniform(const std::string& name, float value);
    void setUniform(const std::string& name, double value);
    void setUniform(const std::string& name, const glm::vec3& value);
    void setUniform(const std::string& name, const glm::vec4& value);
    void setUniform(const std::string& name, const glm::mat4& matrix);
    void setUniform(const std::string& name, const std::vector<glm::vec3>& values);
};

} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_SHADERPROGRAM
