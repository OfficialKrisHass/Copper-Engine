#include "cupch.h"
#include "Engine/Renderer/Texture.h"

#include <glad/glad.h>
#include <stb/stb_image.h>

namespace Copper {
    
    Texture::Texture(uint32 width, uint32 height) {

        glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
        glTexStorage2D(m_id, 1, GL_RGBA8, width, height);

        glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
    }
    Texture::Texture(UVector2I size) : Texture(size.x, size.y) {}
    Texture::Texture(const std::string& path) {

        int32 width, height, channels;
        stbi_set_flip_vertically_on_load(1);

        stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        if(data) {

            m_size.x = width;
            m_size.y = height;

            glGenTextures(1, &m_id);
            glBindTexture(GL_TEXTURE_2D, m_id);
            
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            stbi_image_free(data);
            
        }
        
    }


    
}