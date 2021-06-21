#include "core/TexturePrecomputation.h"

#include <functional>
#include <memory>

#include <GL/glew.h>

#include "core/ShaderProgram.h"
#include "core/Texture.h"

namespace {

// These are already in normalised device coordinate form
constexpr size_t quadVerticesStride = 4 * sizeof(float);
constexpr float quadVertices[] = {
        // Position  // TexCoord
        -1.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f,

        -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f
};

} // anonymous namespace

namespace PBR {

void TexturePrecomputation::renderToTexture(std::shared_ptr<Texture> texture, const ShaderProgram& shaderProgram,
                                            unsigned int width, unsigned int height,
                                            const std::function<void()>& setUniforms)
{
    // Create a texture that we're going to render to
    glBindTexture(GL_TEXTURE_2D, texture->id());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Create and bind the buffer for this render
    unsigned int framebufferId;
    glGenFramebuffers(1, &framebufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->id(), 0);

    // Unbind the texture now that we have finished setting it up
    glBindTexture(GL_TEXTURE_2D, 0);

    /*
     * It's still a shader, so we still need some vertex data to send in. The vertex data
     * defined above is in normalised device coordinates, which the vertex shader
     * essentially just forwards to the fragment shader, where the interesting stuff happens.
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

    // Enable the shader program and set its uniforms using the supplied callback
    glUseProgram(shaderProgram.id());
    setUniforms();

    // Run the shader program
    glViewport(0, 0, width, height);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Delete the frame buffer now that the data is stored in the texture
    glDeleteFramebuffers(1, &framebufferId);

    // We're done so we can unbind and delete the data buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &vboId);
    glDeleteVertexArrays(1, &vaoId);

    // Unbind the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void TexturePrecomputation::renderToMipmappedTexture(std::shared_ptr<Texture> texture,
                                                     const ShaderProgram& shaderProgram,
                                                     unsigned int maxWidth, unsigned int maxHeight,
                                                     unsigned int mipmapLevels,
                                                     const std::function<void(unsigned int)>& setUniforms)
{
    // Allocate memory for the texture we're going to generate
    glBindTexture(GL_TEXTURE_2D, texture->id());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, maxWidth, maxHeight, 0, GL_RGB, GL_FLOAT, nullptr);

    // Set up texture sampling parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipmapLevels);
#ifdef APPLE
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL_APPLE, mipmapLevels);
#endif

    // Allocate memory for the mipmap
    glGenerateMipmap(GL_TEXTURE_2D);

    /*
     * It's still a shader, so we still need some vertex data to send in. The vertex data
     * defined above is in normalised device coordinates, which the vertex shader
     * essentially just forwards to the fragment shader, where the interesting stuff happens.
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

    // Create a framebuffer that we can render to
    unsigned int framebufferId;
    glGenFramebuffers(1, &framebufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);

    // Run the shader for each mipmap level
    for (unsigned int mipmapLevel = 0; mipmapLevel < mipmapLevels; mipmapLevel++) {

        // Enable the shader program
        glUseProgram(shaderProgram.id());

        // Set the uniforms using the passed-in callback
        setUniforms(mipmapLevel);

        // Compute width of this level
        unsigned int width = maxWidth >> mipmapLevel;
        unsigned int height = maxHeight >> mipmapLevel;

        glViewport(0, 0, width, height);

        // Bind the correct mipmap level of the texture to the framebuffer so that this
        // will be the target of the next render
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->id(), mipmapLevel);

        // Run the shader
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 6);

    }

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // We're done so we can unbind and delete the data buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &vboId);
    glDeleteVertexArrays(1, &vaoId);

    // Unbind and delete the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &framebufferId);
}

} // namespace PBR
