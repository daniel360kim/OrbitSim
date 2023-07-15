/**
 * @file Scene.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief 
 * @version 0.1
 * @date 2023-07-14
 * 
 * 
 */

#include "Scene.h"

namespace Visualization
{
    Scene::Scene(uint32_t width, uint32_t height)
        : m_Width(width), m_Height(height)
    {
        m_Image = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
        m_imageBuffer.resize(width * height);
    }

    Scene::~Scene()
    {
    }

    void Scene::OnUpdate(float ts)
    {
        Clear();
        UpdateImage();
    }

    void Scene::OnUIRender()
    {
    }

    void Scene::Clear(uint32_t clearColor)
    {
        for (uint32_t i = 0; i < m_Width * m_Height; i++)
        {
            m_imageBuffer[i] = clearColor;
        }
    }

    void Scene::ResizeIfNeeded(uint32_t width, uint32_t height)
    {
        if (m_Width != width || m_Height != height)
        {
            m_Width = width;
            m_Height = height;

            m_Image = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
            m_imageBuffer.resize(width * height);
        }
    }

    void Scene::UpdateImage()
    {
        m_Image->SetData(m_imageBuffer.data());
    }
}