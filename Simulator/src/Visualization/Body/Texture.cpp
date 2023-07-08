/**
 * @file Texture.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief
 * @version 0.1
 * @date 2023-07-06
 *
 *
 */

#include "Texture.h"

#include <stb_image.h>
#include <iostream>

namespace Visualization
{
    Texture::Texture(const std::string &filepath)
        : m_Width(0), m_Height(0)
    {
        int channels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(filepath.c_str(), &m_Width, &m_Height, &channels, 0);
        if (data)
        {
            glGenTextures(1, &m_TextureID);
            glBindTexture(GL_TEXTURE_2D, m_TextureID);

            // Set the texture wrapping parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            // Set the texture filtering parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // Generate the texture
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            stbi_image_free(data);
        }
        else
        {
            // Handle texture loading error
            std::cerr << "Failed to load texture: " << filepath << std::endl;
        }
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &m_TextureID);
    }

    void Texture::Bind() const
    {
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
    }

    void Texture::Unbind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    glm::vec3 Texture::Sample(float u, float v) const
    {
        if (m_Width == 0 || m_Height == 0)
        {
            return glm::vec3(0.0f, 0.0f, 0.0f); // Return black color if texture data is not available
        }

        // Calculate the texture coordinate position
        int x = static_cast<int>(u * m_Width);
        int y = static_cast<int>(v * m_Height);

        // Make sure the coordinates are within the valid range
        if (x < 0)
            x = 0;
        else if (x >= m_Width)
            x = m_Width - 1;
        if (y < 0)
            y = 0;
        else if (y >= m_Height)
            y = m_Height - 1;

        // Retrieve the color from the texture data
        int index = (y * m_Width + x) * 4; // Assuming 4 channels (RGBA)

        unsigned char *data;
        if (data)
        {
            float r = static_cast<float>(data[index]) / 255.0f;
            float g = static_cast<float>(data[index + 1]) / 255.0f;
            float b = static_cast<float>(data[index + 2]) / 255.0f;

            return glm::vec3(r, g, b);
        }

        return glm::vec3(0.0f, 0.0f, 0.0f); // Return black color if color retrieval fails
    }

}