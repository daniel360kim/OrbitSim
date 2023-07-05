/**
 * @file Renderer.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief
 * @version 0.1
 * @date 2023-07-04
 *
 *
 */

#include "Renderer.h"

#include "Walnut/Random.h"

#include <iostream>

#include "ViewPort.h"

namespace Visualization
{
    Renderer::Renderer(uint32_t width, uint32_t height)
        : m_Width(width), m_Height(height)
    {
        m_Image = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
        m_ImageBuffer = new uint32_t[width * height];
    }

    Renderer::~Renderer()
    {
        delete[] m_ImageBuffer;
    }

    void Renderer::Draw(uint32_t xPos, uint32_t yPos)
    {
        auto earth = ViewPort::Get()->GetEarth();

        uint32_t *earthPixels = earth->GetPixels();
        uint32_t earthWidth = earth->GetWidth();
        uint32_t earthHeight = earth->GetHeight();

        for (uint32_t y = 0; y < earthHeight; y++)
        {
            for (uint32_t x = 0; x < earthWidth; x++)
            {
                int32_t imageX = xPos + x;
                int32_t imageY = yPos + y;

                // Check if the pixel is within the bounds of the m_Image image
                if (imageX >= 0 && imageX < m_Width && imageY >= 0 && imageY < m_Height)
                {
                    uint32_t imageIndex = imageY * m_Width + imageX;
                    uint32_t earthIndex = y * earthWidth + x;

                    m_ImageBuffer[imageIndex] = earthPixels[earthIndex];
                }
            }
        }
    }

    void Renderer::Clear(uint32_t clearColor)
    {
        for (uint32_t i = 0; i < m_Width * m_Height; i++)
        {
            m_ImageBuffer[i] = clearColor;
        }
    }

    void Renderer::ClearRandom()
    {
        for (uint32_t i = 0; i < m_Width * m_Height; i++)
        {
            m_ImageBuffer[i] = Walnut::Random::UInt();
        }
    }

    void Renderer::ResizeIfNeeded(uint32_t width, uint32_t height)
    {
        if (m_Width == width && m_Height == height)
            return;

        m_Width = width;
        m_Height = height;

        m_Image = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);

        delete[] m_ImageBuffer;
        m_ImageBuffer = new uint32_t[width * height];
    }

    void Renderer::UpdateImage()
    {
        m_Image->SetData(m_ImageBuffer);
    }

}