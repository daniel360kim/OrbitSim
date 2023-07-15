/**
 * @file Scene.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief 
 * @version 0.1
 * @date 2023-07-14
 * 
 * 
 */

#pragma once

#include <memory>
#include <vector>

#include "Walnut/Image.h"

namespace Visualization
{
    class Scene
    {
    public:
        Scene(uint32_t width, uint32_t height);
        ~Scene();

        virtual void OnUpdate(float ts);
        virtual void OnUIRender();

        virtual void Clear(uint32_t clearColor = 0xFF000000);
        virtual void ResizeIfNeeded(uint32_t width, uint32_t height);

        uint32_t GetWidth() const { return m_Width; }
        uint32_t GetHeight() const { return m_Height; }

        std::vector<uint32_t> &GetImageBuffer() { return m_imageBuffer; }
        std::shared_ptr<Walnut::Image> GetImage() const { return m_Image; }
    
    protected:
        virtual void UpdateImage();

        std::shared_ptr<Walnut::Image> m_Image;
        std::vector<uint32_t> m_imageBuffer;
        uint32_t m_Width = 0;
        uint32_t m_Height = 0;
    };
}