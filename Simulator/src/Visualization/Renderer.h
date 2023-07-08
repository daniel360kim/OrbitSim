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

#include <memory>

#include "Walnut/Image.h"

#include "Camera.h"
#include "Body/Texture.h"

namespace Visualization
{
    class Renderer
    {
    public:
        Renderer(uint32_t width, uint32_t height);
        ~Renderer();

        void Draw(uint32_t xPos, uint32_t yPos, const Camera &camera);

        void Clear(uint32_t clearColor = 0xFF000000);
        void ClearRandom();

        void ResizeIfNeeded(uint32_t width, uint32_t height);

        void UpdateImage();

        uint32_t GetWidth() const { return m_Width; }
        uint32_t GetHeight() const { return m_Height; }

        uint32_t* GetImageBuffer() const { return m_ImageBuffer; }
        std::shared_ptr<Walnut::Image> GetImage() const { return m_Image; }
    
    private:
        std::shared_ptr<Walnut::Image> m_Image;
        uint32_t* m_ImageBuffer = nullptr;
        uint32_t m_Width = 0;
        uint32_t m_Height = 0;
    };
};










#endif