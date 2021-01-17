#ifndef PHYSICALLYBASEDRENDERER_TEXTURE
#define PHYSICALLYBASEDRENDERER_TEXTURE

#include <filesystem>

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace PBR {

/**
 * Wraps an OpenGL texture object.
 */
class Texture {
private:
    unsigned int textureId;

public:
    /**
     * Create a texture object without initialising it.
     */
    Texture();

    /**
     * Create and initialise a texture by loading from the specified image file.
     *
     * This object assumes ownership of the underlying OpenGL texture object and
     * will safely handle its deletion.
     *
     * @param texturePath The path to the image file
     * @param isHDR Whether the file contains floating-point HDR data rather than
     *              unsigned chars
     * @param wrappingMode The wrapping mode to use
     * @param filteringMode The filtering mode to use
     */
    explicit Texture(const std::filesystem::path& texturePath, bool isHDR = false, bool createMipmap = true);

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    ~Texture();

    unsigned int id() const;
};

} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_TEXTURE
