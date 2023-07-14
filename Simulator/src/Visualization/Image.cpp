/**
 * @file Image.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief creates ImGui image of Image
 * @version 0.1
 * @date 2023-07-04
 * 
 * 
 */

#include "Image.h"

#include <string>
#include <iostream>
#include <stb_image.h>

namespace Visualization
{

    Image::Image(const std::filesystem::path& filePath)
        : m_FilePath(filePath)
    {
        std::string filePathString = filePath.string();
        m_Pixels = (uint32_t*)stbi_load(filePathString.c_str(), &m_Width, &m_Height, nullptr, 4);
        if (!m_Pixels)
        {
            std::cerr << "Failed to load image: " << filePathString << std::endl;
        }
    }   

    Image::~Image()
    {
        stbi_image_free(m_Pixels);
    }


}

