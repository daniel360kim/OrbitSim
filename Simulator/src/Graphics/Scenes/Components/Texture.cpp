/**
 * @file Texture.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief
 * @version 0.1
 * @date 2023-07-08
 *
 *
 */

#include "Texture.h"

#include <iostream>
#include <stdexcept>
#include <stb_image.h>
#include <glm/glm.hpp>

namespace Visualization
{
    Texture::Texture(const std::string &path)
        : m_Path(path), m_imageData(nullptr), m_width(0), m_height(0), m_channels(0)
    {
        stbi_set_flip_vertically_on_load(true);
        m_imageData = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 0);
        if (m_imageData == nullptr)
        {
            throw std::runtime_error("Failed to load texture: " + path);
        }
    }

    Texture::~Texture()
    {
        if (m_imageData != nullptr)
        {
            stbi_image_free(m_imageData);
            m_imageData = nullptr;
        }
    }

    glm::vec4 Texture::getPixel(glm::vec2 &textureCoords) const
    {
        if (m_imageData == nullptr)
        {
            std::cout << "Failed to get pixel: " << m_Path << std::endl;
            return glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
        }

        // Check if the texture coordinates are in bounds
        if (textureCoords.x < 0.0f || textureCoords.x > 1.0f || textureCoords.y < 0.0f || textureCoords.y > 1.0f)
        {
            std::cout << "Texture coordinates out of bounds: " << textureCoords.x << ", " << textureCoords.y << std::endl;
            return glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
        }

        // Calculate the pixel index
        unsigned int x = (unsigned int)(textureCoords.x * m_width);
        unsigned int y = (unsigned int)(textureCoords.y * m_height);
        unsigned int index = (y * m_width + x) * m_channels;

        // Check if the index is out of bounds
        if (index >= m_width * m_height * m_channels)
        {
            return glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
        }

        // Get the pixel data
        unsigned char r = m_imageData[index];
        unsigned char g = m_imageData[index + 1];
        unsigned char b = m_imageData[index + 2];
        unsigned char a = m_imageData[index + 3];

        // Convert to float and return
        return glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
    }

    std::vector<glm::vec4> Texture::getPixels(const std::vector<glm::vec2> &textureCoords) const
    {
        std::vector<glm::vec4> colors;
        colors.reserve(textureCoords.size());

        if (m_imageData == nullptr)
        {
            std::cout << "Failed to get pixel: " << m_Path << std::endl;
            colors.resize(textureCoords.size(), glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
            return colors;
        }

        for (const auto &textureCoord : textureCoords)
        {
            // Check if the texture coordinates are in bounds
            if (textureCoord.x < 0.0f || textureCoord.x > 1.0f || textureCoord.y < 0.0f || textureCoord.y > 1.0f)
            {
                std::cout << "Texture coordinates out of bounds: " << textureCoord.x << ", " << textureCoord.y << std::endl;
                colors.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
                continue;
            }

            // Calculate the pixel index
            unsigned int x = static_cast<unsigned int>(textureCoord.x * m_width);
            unsigned int y = static_cast<unsigned int>(textureCoord.y * m_height);
            unsigned int index = (y * m_width + x) * m_channels;

            // Check if the index is out of bounds
            if (index >= m_width * m_height * m_channels)
            {
                colors.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
                continue;
            }

            // Get the pixel data
            unsigned char r = m_imageData[index];
            unsigned char g = m_imageData[index + 1];
            unsigned char b = m_imageData[index + 2];
            unsigned char a = m_imageData[index + 3];

            // Convert to float and store the color
            colors.push_back(glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f));
        }

        return colors;
    }

}