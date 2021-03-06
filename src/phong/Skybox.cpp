#include "phong/Skybox.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>

#include <stb_image.h>

#include "core/ErrorCodes.h"

namespace fs = std::filesystem;

namespace {

constexpr float skyboxCubeVertices[] = {
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

namespace PBR::phong {

Skybox::Skybox(const std::vector<fs::path>& faceTextures)
        :textureId(), vaoId(), vboId()
{
    assert(faceTextures.size() == 6 && "Skybox constructor expects exactly six file paths");

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

    // Load all six textures
    for (unsigned int i = 0; i < 6; i++) {
        int width, height, numChannels;
        const fs::path filePath = faceTextures[i];
        std::string pathString = filePath.string();
        unsigned char* data = stbi_load(pathString.c_str(), &width, &height, &numChannels, 0);

        if (data) {
            // Copy to GPU
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                    data);
            // Delete now that the copy is complete
            stbi_image_free(data);
        }

        else {
            std::cerr << "Failed to load texture for skybox: " << filePath << std::endl;
            exit((int) ErrorCodes::BadTexture);
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
    glDeleteBuffers(1, &vboId);
    glDeleteVertexArrays(1, &vaoId);
}

} // namespace PBR::phong
