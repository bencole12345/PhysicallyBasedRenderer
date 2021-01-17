#ifndef PHYSICALLYBASEDRENDERER_TEXTUREPRECOMPUTATION
#define PHYSICALLYBASEDRENDERER_TEXTUREPRECOMPUTATION

#include <memory>

#include "core/ShaderProgram.h"
#include "core/Texture.h"

namespace PBR {

struct TexturePrecomputation {
    /**
     * Precomputes a texture using the supplied shader program.
     *
     * @param texture The texture to write to
     * @param shaderProgram The shader program to use
     * @param width The width of the texture, in pixels
     * @param height The height of the texture, in pixels
     * @param setUniforms A function that sets all uniforms required for the shader program
     */
    static void renderToTexture(std::shared_ptr<Texture> texture, const ShaderProgram& shaderProgram,
                                unsigned int width, unsigned int height, const std::function<void()>& setUniforms);

    /**
     * Precomputes each level of a mipmapped texture using the supplied shader program.
     *
     * @param texture The texture to write to
     * @param shaderProgram The shader program to use
     * @param maxWidth The width of the base mipmap level
     * @param maxHeight The height of the base mipmap level
     * @param mipmapLevels The number of mipmap levels to render
     * @param setUniforms A function that sets the uniforms for a given mipmap level
     */
    static void renderToMipmappedTexture(std::shared_ptr<Texture> texture, const ShaderProgram& shaderProgram,
                                         unsigned int maxWidth, unsigned int maxHeight, unsigned int mipmapLevels,
                                         const std::function<void(unsigned int)>& setUniforms);
};

} // namespace PBR

#endif //PHYSICALLYBASEDRENDERER_TEXTUREPRECOMPUTATION
