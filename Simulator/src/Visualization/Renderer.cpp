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
#include <glm/gtx/rotate_vector.hpp>
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

    void Renderer::Draw(uint32_t xPos, uint32_t yPos, const Camera &camera)
    {
        // Draw the sphere on top of the earth, set color to blue for now
        auto sphere = ViewPort::Get()->GetSphere();

        float scale = std::min(m_Width, m_Height) / (2.0f * sphere->GetRadius());
        float xCenter = m_Width / 2.0f;
        float yCenter = m_Height / 2.0f;

        for (uint32_t i = 0; i < sphere->GetPositions().size(); i++)
        {
            glm::vec3 position = sphere->GetPositions()[i];

            // Apply transformations using the Camera properties
            position = position * camera.GetScale();              // Scale the position
            position = glm::rotateX(position, camera.GetPitch()); // Rotate around the x-axis (pitch)
            position = glm::rotateY(position, camera.GetYaw());   // Rotate around the y-axis (yaw)
            position = position + camera.GetPosition();           // Translate the position

            glm::vec2 pixelCoords = glm::vec2(position.x, position.y) * scale + glm::vec2(xCenter, yCenter);

            // Convert the pixel coordinates to array index
            int x = static_cast<int>(pixelCoords.x);
            int y = static_cast<int>(pixelCoords.y);
            int index = y * m_Width + x;

            uint32_t sphereColor = 0xFF0000FF;
            // Check if the pixel is within the bounds of the m_Image image
            if (x >= 0 && x < m_Width && y >= 0 && y < m_Height)
            {
                m_ImageBuffer[index] = sphereColor;
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