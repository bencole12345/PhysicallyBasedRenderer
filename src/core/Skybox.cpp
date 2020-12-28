#include "core/Skybox.h"

#include <iostream>
#include <string_view>
#include <vector>

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

#include <stb_image.h>

namespace {

const char vertexShader[] = "src/shaders/skybox.vert";
const char fragmentShader[] = "src/shaders/skybox.frag";

const float skyboxCubeVertices[] = {
        // Position(x, y, z)

        // Back face
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,

        // Front face
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,

        // Left face
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,

        // Right face
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, -0.5f,

        // Bottom face
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        
        // Top face
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
};

} // anonymous namespace

namespace PBR {

Skybox::Skybox(const std::vector<std::string_view>& faceTextures)
        :textureId(), shaderProgram(vertexShader, fragmentShader), vaoId(), vboId()
{
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

    if (faceTextures.size() != 6) {
        std::cerr << "Skybox constructor expected vector of 6 file paths but instead got length: "
                  << faceTextures.size() << std::endl;
        exit(1);
    }

    // Load all six textures
    for (unsigned int i = 0; i < 6; i++) {
        int width, height, numChannels;
        std::string_view fileName = faceTextures[i];
        unsigned char* data = stbi_load(fileName.data(), &width, &height, &numChannels, 0);

        if (data) {
            // Copy to GPU
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                    data);
            // Delete now that the copy is complete
            stbi_image_free(data);
        }

        else {
            std::cerr << "Failed to load texture for skybox: " << fileName << std::endl;
            exit(1);
        }
    }

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Vertex array buffer
    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);

    // Vertex buffer object
    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);

    // Transfer the vertex data
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxCubeVertices), skyboxCubeVertices, GL_STATIC_DRAW);

    // Set up vertex attribute array
    size_t stride = 3 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*) 0);
    glEnableVertexAttribArray(0);
}

Skybox::~Skybox()
{
    glDeleteTextures(1, &textureId);
}

void Skybox::draw(const Camera& camera)
{
    // Use the shader program
    glUseProgram(shaderProgram.id());

    // Pass in the view and projection matrices
    const auto viewMatrix = camera.getViewMatrix();
    const auto projectionMatrix = camera.getProjectionMatrix();

    // Only do the rotation part of the view matrix's transform
    auto viewMatrixCorrected = glm::mat4(glm::mat3(viewMatrix));

    shaderProgram.setUniform("View", viewMatrixCorrected);
    shaderProgram.setUniform("Projection", projectionMatrix);

    // Save the old depth culling mode
    GLint oldDepthFunc;
    GLint oldCullFace;
    glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFunc);
    glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFace);

    // We need less than or equal, because both the skybox and the unrendered
    // pixels will be at the maximum z-buffer value.
    glDepthFunc(GL_LEQUAL);

    // We show the front rather than the back because we're inside the cube now.
    glCullFace(GL_FRONT);

    // Render the skybox
    glBindVertexArray(vaoId);
    int count = sizeof(skyboxCubeVertices) / (3 * sizeof(float));
    glDrawArrays(GL_TRIANGLES, 0, count);

    // Restore the old face culling settings.
    glDepthFunc(oldDepthFunc);
    glCullFace(oldCullFace);
}

} // namespace PBR
