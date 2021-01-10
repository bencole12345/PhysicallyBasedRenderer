#ifndef PHYSICALLYBASEDRENDERER_TEXTURE
#define PHYSICALLYBASEDRENDERER_TEXTURE

#include <filesystem>

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace PBR {

class Texture {
private:
    unsigned int textureId;

public:
    Texture(const std::filesystem::path& texturePath, GLenum wrappingMode = GL_REPEAT, GLenum filteringMode = GL_LINEAR);

    Texture(const Texture &) = delete;
    Texture& operator=(const Texture &) = delete;

    ~Texture();

    unsigned int id() const;
};

} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_TEXTURE
