#ifndef PHYSICALLYBASEDRENDERER_SKYBOX
#define PHYSICALLYBASEDRENDERER_SKYBOX

#include <filesystem>
#include <vector>

#include "core/Camera.h"
#include "core/ShaderProgram.h"

namespace PBR::skybox {

class Skybox {
private:
    unsigned int textureId;
    unsigned int vaoId;
    unsigned int vboId;

public:
    explicit Skybox(const std::vector<std::filesystem::path>& faceTextures);
    ~Skybox();

    /**
     * @return The texture ID returned from OpenGL
     */
    unsigned int getTextureId() const;

    /**
     * @return The vertex array object returned from OpenGL
     */
    unsigned int getVaoId() const;

    /**
     * @return The vertex buffer object returned from OpenGL
     */
    unsigned int getVboId() const;

    /**
     * @return The number of vertices in the buffer
     */
    constexpr size_t numVerticesInBuffer() const;
};

inline
unsigned int Skybox::getTextureId() const
{
    return textureId;
}

inline
unsigned int Skybox::getVaoId() const
{
    return vaoId;
}

inline
unsigned int Skybox::getVboId() const
{
    return vboId;
}

constexpr size_t Skybox::numVerticesInBuffer() const
{
    return 36;
}

} // namespace PBR::skybox

#endif //PHYSICALLYBASEDRENDERER_SKYBOX
