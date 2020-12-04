#ifndef PHYSICALLYBASEDRENDERER_SHADER
#define PHYSICALLYBASEDRENDERER_SHADER

#include <string>

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

namespace PBR {

class BadShaderException : public std::exception {

};

class Shader {
private:
    unsigned int shaderId;

public:
    Shader(const std::string& file, GLenum shaderType);
    ~Shader();

    unsigned int& id();
};

}

#endif //PHYSICALLYBASEDRENDERER_SHADER
