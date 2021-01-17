#ifndef PHYSICALLYBASEDRENDERER_SHADERPROGRAM
#define PHYSICALLYBASEDRENDERER_SHADERPROGRAM

#include <filesystem>
#include <memory>
#include <vector>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "core/ShaderProgram.h"
#include "core/Texture.h"

namespace PBR {

// Forward-declared to avoid circular dependency
namespace skybox { class Skybox; }

/**
 * Wraps a shader program containing a vertex and fragment shader.
 */
class ShaderProgram {
private:
    /**
     * The ID of the shader program created with OpenGL.
     */
    const unsigned int shaderProgramId;

    /**
     * The number of textures added so far.
     */
    unsigned int texturesCount{0};

public:
    ShaderProgram(const std::filesystem::path& vertexShaderLocation, const std::filesystem::path& fragmentShaderLocation);
    ~ShaderProgram();

    /**
     * @return The ID of the shader program created with OpenGL
     */
    unsigned int id() const;

    /**
     * Resets the uniforms that have been passed to this shader program, including textures.
     */
    void resetUniforms();

    void setUniform(const std::string& name, bool value);
    void setUniform(const std::string& name, float value);
    void setUniform(const std::string& name, double value);
    void setUniform(const std::string& name, int value);
    void setUniform(const std::string& name, const glm::vec3& value);
    void setUniform(const std::string& name, const glm::vec4& value);
    void setUniform(const std::string& name, const glm::mat4& matrix);
    void setUniform(const std::string& name, const std::vector<float>& values);
    void setUniform(const std::string& name, const std::vector<glm::vec3>& values);
    void setUniform(const std::string& name, const std::shared_ptr<Texture>& texture);
    void setUniform(const std::string& name, const std::shared_ptr<skybox::Skybox>& skybox);

};

} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_SHADERPROGRAM
