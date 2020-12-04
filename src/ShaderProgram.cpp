#include "ShaderProgram.h"

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

namespace PBR {

ShaderProgram::ShaderProgram(Shader& vertexShader, Shader& fragmentShader)
: shaderProgramId(glCreateProgram()), vertexShader(vertexShader), fragmentShader(fragmentShader)
{
    glAttachShader(shaderProgramId, vertexShader.id());
    glAttachShader(shaderProgramId, fragmentShader.id());

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

} // namespace PBR
