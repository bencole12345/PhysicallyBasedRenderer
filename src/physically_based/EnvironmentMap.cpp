#include "physically_based/EnvironmentMap.h"

#include <filesystem>
#include <memory>
#include <optional>

#include <GL/glew.h>

#include "core/DirectedLightSource.h"
#include "core/ShaderProgram.h"
#include "core/Texture.h"
#include "physically_based/Util.h"

namespace fs = std::filesystem;

namespace PBR::physically_based {

namespace {

// These are already in normalised device coordinate form
constexpr size_t quadVerticesStride = 4 * sizeof(float);
constexpr float quadVertices[] = {
        // Position    // TexCoord
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
};

/**
 * Precomputes the irradiance map of the background and returns it as a `Texture`.
 */
std::shared_ptr<Texture> precomputeDiffuseIrradianceMap(std::shared_ptr<Texture> background)
{
    // Create and bind the buffer
    unsigned int framebufferId;
    glGenFramebuffers(1, &framebufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);

    // Create a texture that we're going to render to
    std::shared_ptr<Texture> texture(new Texture());
    glBindTexture(GL_TEXTURE_2D, texture->id());

    // Allocate the memory for the texture (but don't worry about copying any data
    // over, that'll happen when we render to it). We can use a low resolution because
    // the data is going to be blurry anyway.
    int width = 32;
    int height = 32;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glViewport(0, 0, width, height);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Unbind the texture now that we have finished setting it up
    glBindTexture(GL_TEXTURE_2D, 0);

    // Attach the texture to the framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->id(), 0);

    // Load the shader program we need to use
    auto precomputeIrradianceMapVertexShader =
            Util::getPhysicallyBasedShadersDirectory() / "precompute_irradiance_map.vert";
    auto precomputeIrradianceMapFragmentShader =
            Util::getPhysicallyBasedShadersDirectory() / "precompute_irradiance_map.frag";
    ShaderProgram shader(precomputeIrradianceMapVertexShader, precomputeIrradianceMapFragmentShader);

    /*
     * It's still a shader, so we still need some data to send in. The vertex data
     * defined above is in normalised device coordinates, which the vertex shader
     * essentially just forwards to the fragment shader, which is where the interesting
     * stuff happens.
     *
     * We still have to create buffers for the vertex data to send to the GPU. The difference
     * is that we're only going to need it once (this is for precomputation rather than
     * running once per frame), so we'll delete the buffers again as soon as the render
     * is complete.
     */

    // Set up buffers for the vertex data we're going to send
    unsigned int vaoId, vboId;
    glGenVertexArrays(1, &vaoId);
    glGenBuffers(1, &vboId);

    // Copy data into the buffers
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    // Set up the vertex attributes
    glBindVertexArray(vaoId);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, quadVerticesStride, (void*) (0 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, quadVerticesStride, (void*) (2 * sizeof(float)));

    // Set up the shader program
    glUseProgram(shader.id());
    shader.setUniform("backgroundTexture", background);

    // Run the shader
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // We're done so we can unbind and delete the data buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &vboId);
    glDeleteVertexArrays(1, &vaoId);

    // Unbind the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Restore the screen dimensions (we don't need to worry about whether the window
    // has been resized because this happens before the window is visible)
    glViewport(0, 0, 800, 600);

    // Delete the frame buffer now that the data is stored in the texture
    glDeleteFramebuffers(1, &framebufferId);

    return texture;
}

} // anonymous namespace

EnvironmentMap::EnvironmentMap(const fs::path& texturePath,
                               std::optional<DirectedLightSource> sun)
        :backgroundTexture(new Texture(texturePath, true)),
         diffuseIrradianceMap(precomputeDiffuseIrradianceMap(backgroundTexture)),
         specularIrradianceMap(backgroundTexture), // TODO: Compute this properly
         sun(sun)
{
}

std::shared_ptr<Texture> EnvironmentMap::getBackgroundTexture() const
{
    return backgroundTexture;
}

std::shared_ptr<Texture> EnvironmentMap::getDiffuseIrradianceMapTexture() const
{
    return diffuseIrradianceMap;
}

std::shared_ptr<Texture> EnvironmentMap::getSpecularIrradianceMapTexture() const
{
    return specularIrradianceMap;
}

const std::optional<DirectedLightSource>& EnvironmentMap::getSun() const
{
    return sun;
}

} // namespace PBR::physically_based
