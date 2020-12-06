#include "ShaderProgram.h"

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

namespace PBR {

ShaderProgram::ShaderProgram(const std::shared_ptr<Shader>& vertexShader, const std::shared_ptr<Shader>& fragmentShader)
        :shaderProgramId(glCreateProgram()), vertexShader(vertexShader), fragmentShader(fragmentShader)
{
    // TODO: Scrap the shader class and load the shaders in here so that we can delete them when
    //      we're done

    glAttachShader(shaderProgramId, vertexShader->id());
    glAttachShader(shaderProgramId, fragmentShader->id());

    glLinkProgram(shaderProgramId);

    // TODO: Handle deleting the shaders here
    //    glDeleteShader(vertexShader);
    //    glDeleteShader(fragmentShader);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgramId, 512, nullptr, infoLog);
        fputs(infoLog, stderr);
    }
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

void ShaderProgram::setUniform(const std::string& name, glm::vec3 value)
{
    int position = glGetUniformLocation(shaderProgramId, name.c_str());
    glUniform3f(position, value[0], value[1], value[2]);
}

void ShaderProgram::setUniform(const std::string& name, glm::vec4 value)
{
    int position = glGetUniformLocation(shaderProgramId, name.c_str());
    glUniform4f(position, value[0], value[1], value[2], value[3]);
}

void ShaderProgram::setUniform(const std::string& name, const glm::mat4& matrix)
{
    int position = glGetUniformLocation(shaderProgramId, name.c_str());
    glUniformMatrix4fv(position, 1, GL_FALSE, &matrix[0][0]);
}

} // namespace PBR
