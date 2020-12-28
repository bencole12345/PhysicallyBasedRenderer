#include "core/ShaderProgram.h"

#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace {

unsigned int loadAndCompileShader(std::string_view shaderLocation, GLenum shaderType)
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
        int bufferSize = 512;
        char infoLog[bufferSize];
        glGetShaderInfoLog(shaderId, bufferSize, nullptr, infoLog);
        std::cerr << "Shader error: " << infoLog << std::endl;
        exit(1);
    }

    return shaderId;
}

} // anonymous namespace

namespace PBR {

ShaderProgram::ShaderProgram(std::string_view vertexShaderLocation, std::string_view fragmentShaderLocation)
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
        int bufferSize = 512;
        char infoLog[bufferSize];
        glGetProgramInfoLog(shaderProgramId, bufferSize, nullptr, infoLog);
        std::cerr << "Error linking shader programs: " << infoLog << std::endl;
        exit(2);
    }

    // We don't need the individual shaders any more
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

unsigned int ShaderProgram::id() const
{
    return shaderProgramId;
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(shaderProgramId);
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

void ShaderProgram::setUniform(const std::string& name, const std::vector<glm::vec3>& values)
{
    int position = glGetUniformLocation(shaderProgramId, name.c_str());
    glUniform3fv(position, values.size(), reinterpret_cast<const GLfloat*>(&values[0]));
}

} // namespace PBR
