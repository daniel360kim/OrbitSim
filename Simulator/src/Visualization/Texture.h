/**
 * @file Texture.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief Base class to load textures
 * @version 0.1
 * @date 2023-07-08
 * 
 * 
 */

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace Visualization
{
    class Texture
    {
    public:
        Texture(const std::string& path);
        ~Texture();

        glm::vec4 getPixel(glm::vec2& textureCoords) const;

        inline int getWidth() const { return m_width; }
        inline int getHeight() const { return m_height; }
    private:
        std::string m_Path;
        unsigned char* m_imageData;

        int m_width;
        int m_height;
        int m_channels;

    };
}






#endif
