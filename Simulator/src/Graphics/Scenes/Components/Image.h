/**
 * @file Image.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief creates ImGui image of Image
 * @version 0.1
 * @date 2023-07-04
 * 
 * 
 */

#pragma once

#include <filesystem>

namespace Visualization
{
    class Image
    {
    public:
        Image(const std::filesystem::path& filePath);
        ~Image();

        uint32_t* GetPixels() const { return m_Pixels; }
        int GetWidth() const { return m_Width; }
        int GetHeight() const { return m_Height; }
        
    private:
        uint32_t* m_Pixels = nullptr;
        std::filesystem::path m_FilePath;

        int m_Width = 0;
        int m_Height = 0;
    };
};

