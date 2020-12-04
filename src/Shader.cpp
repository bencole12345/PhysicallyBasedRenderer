#include "Shader.h"

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#include <string>
#include <fstream>
#include <iostream>

namespace PBR {

Shader::Shader(const std::string& filepath, GLenum shaderType)
        :shaderId(glCreateShader(shaderType))
{
    // Read the file
    std::ifstream t(filepath);
    std::string shaderSource((std::istreambuf_iterator<char>(t)),std::istreambuf_iterator<char>());

    // We need the address of the pointer so store it on the stack
    const char *shaderSourcePtr = shaderSource.c_str();

    // Compile the shader
    int count = 1;
    glShaderSource(shaderId, count, &shaderSourcePtr, nullptr);
    glCompileShader(shaderId);
    int success;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
        std::cerr << "Shader error: " << infoLog << std::endl;
        throw BadShaderException();
    }
}

Shader::~Shader()
{
    glDeleteShader(shaderId);
}

unsigned int& Shader::id()
{
    return shaderId;
}

} // namespace PBR
