#include "debug/DebuggingUtil.h"

#include <filesystem>
#include <memory>

#include <GL/glew.h>

#include "core/ShaderProgram.h"
#include "core/Texture.h"

namespace fs = std::filesystem;

namespace PBR::debug {

namespace {

// These are already in normalised device coordinate form
constexpr size_t quadVerticesStride = 4 * sizeof(float);
constexpr float quadVertices[] = {
        // Position  // TexCoord
        0.2f, -0.2f, 0.0f, 1.0f,
        0.2f, -0.8f, 0.0f, 0.0f,
        0.8f, -0.8f, 1.0f, 0.0f,

        0.2f, -0.2f, 0.0f, 1.0f,
        0.8f, -0.8f, 1.0f, 0.0f,
        0.8f, -0.2f, 1.0f, 1.0f
};

} // anonymous namespace

void renderTextureToBottomCorner(std::shared_ptr<Texture> texture, bool isHDR, bool useMipmapSampling, float lod)
{
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

    // Enable the shader and prepare its uniforms
    auto vertexShader = fs::current_path() / "src" / "debug" / "shaders" / "DisplayTexture.vert";
    auto fragmentShader = fs::current_path() / "src" / "debug" / "shaders" / "DisplayTexture.frag";
    ShaderProgram shaderProgram(vertexShader, fragmentShader);
    glUseProgram(shaderProgram.id());
    shaderProgram.setUniform("textureToDisplay", texture);
    shaderProgram.setUniform("isHDR", isHDR);
    shaderProgram.setUniform("useMipmapSampling", useMipmapSampling);
    shaderProgram.setUniform("lod", lod);

    // Run the shader
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // We're done so we can unbind and delete the data buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &vboId);
    glDeleteVertexArrays(1, &vaoId);
}

} // namespace PBR::debug
