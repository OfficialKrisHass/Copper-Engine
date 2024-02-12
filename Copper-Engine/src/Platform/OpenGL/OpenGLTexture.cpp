#include "cupch.h"
#include "Engine/Renderer/Texture.h"

#include <glad/glad.h>
#include <stb/stb_image.h>

namespace Copper {

    void Texture::Create(uint32 width, uint32 height, Format format, uint8* pixels, const std::string& name) {

        if (m_id != 0)
            Delete();

        m_size.x = width;
        m_size.y = height;

        m_name = name;

        // Create the texture

        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);

        // Setup texture parameters

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Create the texture

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB + (uint8) format, GL_UNSIGNED_BYTE, pixels);
        glGenerateMipmap(GL_TEXTURE_2D);

    }
    void Texture::Create(const std::string& filePath, Format format) {

        stbi_set_flip_vertically_on_load(1);

        int32 width, height, channels;
        uint8* pixels = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
        if (!pixels) {

            LogError("Couldn't read image pixels! Path: {}", filePath);
            return;

        }

        fs::path path = filePath;
        Create(width, height, format, pixels, path.filename().string());

        stbi_image_free(pixels);

    }

    void Texture::Delete() {

        glDeleteTextures(1, &m_id);
        m_size = UVector2I::zero;

    }

    void Texture::Bind(uint32 unit) const {
        
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, m_id);
    
    }
    void Texture::Unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }
    
}