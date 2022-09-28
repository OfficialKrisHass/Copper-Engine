#include "cupch.h"
#include "Engine/Renderer/Texture.h"

#include <glad/glad.h>
#include <stb/stb_image.h>

namespace Copper {
    
    Texture::Texture(uint32_t width, uint32_t height) {

        glCreateTextures(GL_TEXTURE_2D, 1, &ID);
        glTexStorage2D(ID, 1, GL_RGBA8, width, height);

        glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(ID, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
    }
    Texture::Texture(UVector2I size) : Texture(size.x, size.y) {}
    Texture::Texture(const std::string& path) {

        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);

        stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        if(data) {

            size.x = width;
            size.y = height;

            glGenTextures(1, &ID);
            glBindTexture(GL_TEXTURE_2D, ID);
            
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            //glTextureSubImage2D(ID, 0, 0, 0, size.x, size.y, GL_RGBA, GL_UNSIGNED_BYTE, data);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            stbi_image_free(data);
            
        }
        
    }


    
}