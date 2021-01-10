#include "core/Texture.h"

#include <filesystem>
#include <iostream>

#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "core/ErrorCodes.h"

namespace fs = std::filesystem;

namespace PBR {

Texture::Texture(const fs::path& texturePath, GLenum wrappingMode, GLenum filteringMode)
    : textureId()
{
    // Load the image
    int width, height, numChannels;
    std::string path = texturePath.string();
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &numChannels, 0);

    if (!data) {
        std::cerr << "Failed to load texture: " << texturePath << std::endl;
        exit((int) ErrorCodes::BadTexture);
    }

    // Create an OpenGL texture object
    glGenTextures(1, &textureId);

    // Generate mipmap
    glGenerateMipmap(textureId);

    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, textureId);

    // Set the wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrappingMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrappingMode);

    // Set the filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filteringMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filteringMode);

    // Write the texture data to the GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    // Delete the image data now that it has been copied
    stbi_image_free(data);
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
