#include "core/Texture.h"

#include <filesystem>
#include <iostream>
#include <variant>

#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "core/ErrorCodes.h"

namespace fs = std::filesystem;

namespace PBR {

Texture::Texture()
        :textureId()
{
    glGenTextures(1, &textureId);
}

Texture::Texture(const fs::path& texturePath, bool isHDR, bool createMipmap)
        :Texture()
{
    // Load the image
    int width, height, numChannels;
    std::string path = texturePath.string();
    std::variant<unsigned char*, float*> data;
    bool success;

    if (isHDR) {
        stbi_set_flip_vertically_on_load(true);
        data = stbi_loadf(path.c_str(), &width, &height, &numChannels, 0);
        stbi_set_flip_vertically_on_load(false);
        success = std::get<float*>(data) != nullptr;
    }
    else {
        data = stbi_load(path.c_str(), &width, &height, &numChannels, 0);
        success = std::get<unsigned char*>(data) != nullptr;
    }

    if (!success) {
        std::cerr << "Failed to load texture: " << texturePath << std::endl;
        exit((int) ErrorCodes::BadTexture);
    }

    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, textureId);

    // Set the wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set the filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // Write the texture data to the GPU and delete the copy stored here afterwards
    if (isHDR) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, std::get<float*>(data));
        stbi_image_free(std::get<float*>(data));
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                     std::get<unsigned char*>(data));
        stbi_image_free(std::get<unsigned char*>(data));
    }

    // Generate mipmap
    if (createMipmap) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
    glDeleteTextures(1, &textureId);
}

unsigned int Texture::id() const
{
    return textureId;
}

} // namespace PBR
