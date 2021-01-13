#include "core/ShaderProgram.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "core/ErrorCodes.h"
#include "core/Texture.h"
#include "skybox/Skybox.h"

namespace fs = std::filesystem;

namespace PBR {

namespace {

unsigned int loadAndCompileShader(const fs::path& shaderLocation, GLenum shaderType)
{
    // Read the file
    std::ifstream stream(shaderLocation);
    std::string shaderSource((std::istreambuf_iterator<char>(stream)),
            std::istreambuf_iterator<char>());

    // Store the char* in an lvalue so we can pass its address to glShaderSource()
    const char* shaderSourcePtr = shaderSource.c_str();

    // The ID of the shader we are creating
    unsigned int shaderId = glCreateShader(shaderType);

    // Compile the shader
    int count = 1;
    glShaderSource(shaderId, count, &shaderSourcePtr, nullptr);
    glCompileShader(shaderId);

    // Check it was successful
    int success;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
        constexpr int bufferSize = 512;
        char infoLog[bufferSize];
        glGetShaderInfoLog(shaderId, bufferSize, nullptr, infoLog);
        std::cerr << "Shader error: " << infoLog << std::endl;
        exit((int) ErrorCodes::BadShaderProgram);
    }

    return shaderId;
}

} // anonymous namespace

ShaderProgram::ShaderProgram(const fs::path& vertexShaderLocation, const fs::path& fragmentShaderLocation)
        :shaderProgramId(glCreateProgram())
{
    // Load the shaders
    unsigned int vertexShader = loadAndCompileShader(vertexShaderLocation, GL_VERTEX_SHADER);
    unsigned int fragmentShader = loadAndCompileShader(fragmentShaderLocation, GL_FRAGMENT_SHADER);

    // Create the combined shader program
    glAttachShader(shaderProgramId, vertexShader);
    glAttachShader(shaderProgramId, fragmentShader);
    glLinkProgram(shaderProgramId);

    // Check it was successful
    int success;
    glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);
    if (!success) {
        constexpr int bufferSize = 512;
        char infoLog[bufferSize];
        glGetProgramInfoLog(shaderProgramId, bufferSize, nullptr, infoLog);
        std::cerr << "Error linking shader programs: " << infoLog << std::endl;
        exit((int) ErrorCodes::FailedToLinkShaders);
    }

    // We don't need the individual shaders any more
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(shaderProgramId);
}

unsigned int ShaderProgram::id() const
{
    return shaderProgramId;
}

void ShaderProgram::resetUniforms()
{
    // Unbind all textures
    for (unsigned int i = 0; i < texturesCount; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // Reset the number of textures bound
    texturesCount = 0;
}

void ShaderProgram::setUniform(const std::string& name, float value)
{
    int position = glGetUniformLocation(shaderProgramId, name.c_str());
    glUniform1f(position, value);
}

void ShaderProgram::setUniform(const std::string& name, double value)
{
    int position = glGetUniformLocation(shaderProgramId, name.c_str());
    glUniform1d(position, value);
}

void ShaderProgram::setUniform(const std::string& name, int value)
{
    int position = glGetUniformLocation(shaderProgramId, name.c_str());
    glUniform1i(position, value);
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec3& value)
{
    int position = glGetUniformLocation(shaderProgramId, name.c_str());
    glUniform3f(position, value[0], value[1], value[2]);
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec4& value)
{
    int position = glGetUniformLocation(shaderProgramId, name.c_str());
    glUniform4f(position, value[0], value[1], value[2], value[3]);
}

void ShaderProgram::setUniform(const std::string& name, const glm::mat4& matrix)
{
    int position = glGetUniformLocation(shaderProgramId, name.c_str());
    glUniformMatrix4fv(position, 1, GL_FALSE, &matrix[0][0]);
}

void ShaderProgram::setUniform(const std::string& name, const std::vector<float>& values)
{
    int position = glGetUniformLocation(shaderProgramId, name.c_str());
    glUniform1fv(position, values.size(), &values[0]);
}

void ShaderProgram::setUniform(const std::string& name, const std::vector<glm::vec3>& values)
{
    int position = glGetUniformLocation(shaderProgramId, name.c_str());
    glUniform3fv(position, values.size(), reinterpret_cast<const GLfloat*>(&values[0]));
}

void ShaderProgram::setUniform(const std::string& name, const std::shared_ptr<Texture>& texture)
{
    unsigned int textureUnit = texturesCount++;
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, texture->id());
    setUniform(name, (int)textureUnit);
}

void ShaderProgram::setUniform(const std::string& name, const std::shared_ptr<skybox::Skybox>& skybox)
{
    unsigned int textureUnit = texturesCount++;
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getTextureId());
    setUniform(name, (int)textureUnit);
}

} // namespace PBR
