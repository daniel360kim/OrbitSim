/**
 * @file Renderer.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief 
 * @version 0.1
 * @date 2023-07-04
 * 
 * 
 */

#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <memory>

#include "Walnut/Image.h"

#include "Camera.h"
#include "Texture.h"

namespace Visualization
{
    class Renderer
    {
    public:
        Renderer(uint32_t width, uint32_t height);
        ~Renderer();

        void Draw();

        void Clear(uint32_t clearColor = 0xFF000000);
        void ClearRandom();

        void ResizeIfNeeded(uint32_t width, uint32_t height);

        void UpdateImage();

        uint32_t GetWidth() const { return m_Width; }
        uint32_t GetHeight() const { return m_Height; }

        std::vector<uint32_t>& GetImageBuffer() { return m_imageBuffer; }
        std::shared_ptr<Walnut::Image> GetImage() const { return m_Image; }
    
    private:
        std::shared_ptr<Walnut::Image> m_Image;
        
        std::vector<uint32_t> m_imageBuffer;

        void fillTriangle(int index1, int index2, int index3, uint32_t color1, uint32_t color2, uint32_t color3);
        uint32_t interpolateColor(uint32_t color1, uint32_t color2, float t);

        uint32_t m_Width = 0;
        uint32_t m_Height = 0;

    };
};










#endif