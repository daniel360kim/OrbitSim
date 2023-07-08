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
#include <unordered_map>
#include <glm/gtx/rotate_vector.hpp>
#include "ViewPort.h"

namespace Visualization
{
    Renderer::Renderer(uint32_t width, uint32_t height)
        : m_Width(width), m_Height(height)
    {
        m_Image = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
        m_imageBuffer.resize(width * height);
    }

    Renderer::~Renderer()
    {
    }

    void Renderer::Clear(uint32_t clearColor)
    {
        for (uint32_t i = 0; i < m_Width * m_Height; i++)
        {
            m_imageBuffer[i] = clearColor;
        }
    }

    void Renderer::ClearRandom()
    {
        for (uint32_t i = 0; i < m_Width * m_Height; i++)
        {
            m_imageBuffer[i] = Walnut::Random::UInt();
        }
    }

    void Renderer::ResizeIfNeeded(uint32_t width, uint32_t height)
    {
        if (m_Width == width && m_Height == height)
            return;

        m_Width = width;
        m_Height = height;

        m_Image = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);

        m_imageBuffer.resize(width * height);
    }

    void Renderer::UpdateImage()
    {
        // m_Image->SetData(m_ImageBuffer);
        m_Image->SetData(m_imageBuffer.data());
    }

    void Renderer::Draw(std::vector<std::shared_ptr<Body>>& bodies)
    {
        DrawBackground(ViewPort::Get()->GetSpaceBackground());
        // Draw the earth on top of the earth, set color to blue for now
        auto earth = ViewPort::Get()->GetEarth();
        auto camera = ViewPort::Get()->GetCamera();

        // Divide the available space into the number of bodies
        //Distribute each 
        // Calculate the offset for each body

        int numBodies = bodies.size();

        float dividerDistance = m_Width / (numBodies + 1);
        
        for (int i = 0; i < numBodies; i++)
        {
            float offset = (i + 1) * dividerDistance;
            std::cout << offset << std::endl;

            std::cout << "Middle of screen: " << m_Width / 2 << std::endl;
            DrawBody(bodies[i], camera, glm::vec2(offset, m_Height / 2));
        }
    }

    void Renderer::DrawBackground(std::shared_ptr<Image> background)
    {
        // Draw the space background
        uint32_t *backgroundData = background->GetPixels();
        uint32_t backgroundWidth = background->GetWidth();
        uint32_t backgroundHeight = background->GetHeight();

        for (uint32_t y = 0; y < m_Height; y++)
        {
            for (uint32_t x = 0; x < m_Width; x++)
            {
                uint32_t backgroundX = (uint32_t)((float)x / m_Width * backgroundWidth);
                uint32_t backgroundY = (uint32_t)((float)y / m_Height * backgroundHeight);

                uint32_t backgroundIndex = backgroundY * backgroundWidth + backgroundX;

                m_imageBuffer[y * m_Width + x] = backgroundData[backgroundIndex];
            }
        }
    }

    void Renderer::DrawBody(std::shared_ptr<Body> body, std::shared_ptr<Camera> camera, glm::vec2& offset)
    {
        float scale = std::min(m_Width, m_Height) / (2.0f * body->GetRadius());

        const std::vector<unsigned int> &indices = body->GetIndices();
        const std::vector<glm::vec3> &positions = body->GetPositions();
        const std::vector<glm::vec2> &texCoords = body->GetTexCoords();

        glm::mat4 viewMatrix = camera->GetViewMatrix();
        CameraInfo cameraInfo = camera->GetCameraInfo();

        #pragma omp parallel for
        for (uint32_t i = 0; i < indices.size(); i += 3)
        {
            glm::vec3 position1 = positions[indices[i]];
            glm::vec3 position2 = positions[indices[i + 1]];
            glm::vec3 position3 = positions[indices[i + 2]];

            // Apply transformations using the Camera properties
            position1 = glm::vec3(viewMatrix * glm::vec4(position1 * cameraInfo.scale, 1.0));
            position2 = glm::vec3(viewMatrix * glm::vec4(position2 * cameraInfo.scale, 1.0));
            position3 = glm::vec3(viewMatrix * glm::vec4(position3 * cameraInfo.scale, 1.0));

            // Check if the positions are in front of the camera - only render what the camera can see
            glm::vec3 cameraToPosition1 = position1 - cameraInfo.position;
            glm::vec3 cameraToPosition2 = position2 - cameraInfo.position;
            glm::vec3 cameraToPosition3 = position3 - cameraInfo.position;

            if (isInFrontOfCamera(cameraToPosition1, cameraToPosition2, cameraToPosition3, cameraInfo.forwardDirection))
            {
                glm::vec2 pixelCoords1 = transformToPixelCoords(position1, scale, offset);
                glm::vec2 pixelCoords2 = transformToPixelCoords(position2, scale, offset);
                glm::vec2 pixelCoords3 = transformToPixelCoords(position3, scale, offset);

                // Convert the pixel coordinates to array indices
                int x1 = static_cast<int>(pixelCoords1.x);
                int y1 = static_cast<int>(pixelCoords1.y);
                int index1 = y1 * m_Width + x1;

                int x2 = static_cast<int>(pixelCoords2.x);
                int y2 = static_cast<int>(pixelCoords2.y);
                int index2 = y2 * m_Width + x2;

                int x3 = static_cast<int>(pixelCoords3.x);
                int y3 = static_cast<int>(pixelCoords3.y);
                int index3 = y3 * m_Width + x3;

                // Check if the pixels are within the bounds of the m_Image image
                if (isWithinImageBounds(x1, y1) &&
                    isWithinImageBounds(x2, y2) &&
                    isWithinImageBounds(x3, y3))
                {
                    // Map texture coordinates to the triangle vertices
                    glm::vec2 texCoords1 = texCoords[indices[i]];
                    glm::vec2 texCoords2 = texCoords[indices[i + 1]];
                    glm::vec2 texCoords3 = texCoords[indices[i + 2]];

                    // Sample the texture colors
                    glm::vec4 color1 = body->GetTexture()->getPixel(glm::vec2(texCoords1.x, texCoords1.y));
                    glm::vec4 color2 = body->GetTexture()->getPixel(glm::vec2(texCoords2.x, texCoords2.y));
                    glm::vec4 color3 = body->GetTexture()->getPixel(glm::vec2(texCoords3.x, texCoords3.y));

                    // Convert the colors to 32-bit integers
                    uint32_t colorInt1 = convertColors(color1);
                    uint32_t colorInt2 = convertColors(color2);
                    uint32_t colorInt3 = convertColors(color3);

                    // Fill the triangle with the corresponding colors
                    fillTriangle(index1, index2, index3, colorInt1, colorInt2, colorInt3);
                }
            }
        }
    }

    glm::vec2 Renderer::transformToPixelCoords(glm::vec3 positionCoords, float scale, glm::vec2 offset)
    {
        return glm::vec2(positionCoords.x, positionCoords.y) * scale + offset;
    }

    uint32_t Renderer::convertColors(const glm::vec4 &color)
    {
        uint32_t colorInt = (static_cast<uint32_t>(color.a * 255.0f) << 24) |
                            (static_cast<uint32_t>(color.b * 255.0f) << 16) |
                            (static_cast<uint32_t>(color.g * 255.0f) << 8) |
                            (static_cast<uint32_t>(color.r * 255.0f));

        return colorInt;
    }

    void Renderer::fillTriangle(int index1, int index2, int index3, uint32_t color1, uint32_t color2, uint32_t color3)
    {
        // Sort the vertices vertically by y-coordinate (top to bottom)
        if (index2 < index1)
        {
            std::swap(index1, index2);
            std::swap(color1, color2);
        }
        if (index3 < index2)
        {
            std::swap(index2, index3);
            std::swap(color2, color3);
        }
        if (index2 < index1)
        {
            std::swap(index1, index2);
            std::swap(color1, color2);
        }

        int x1 = index1 % m_Width;
        int y1 = index1 / m_Width;

        int x2 = index2 % m_Width;
        int y2 = index2 / m_Width;

        int x3 = index3 % m_Width;
        int y3 = index3 / m_Width;

        // Calculate the slopes of the edges
        float slope1 = static_cast<float>(x2 - x1) / (y2 - y1);
        float slope2 = static_cast<float>(x3 - x1) / (y3 - y1);
        float slope3 = static_cast<float>(x3 - x2) / (y3 - y2);

        // Draw the triangle scanlines
        for (int y = y1; y <= y3; y++)
        {
            int startX = static_cast<int>(x1 + (y - y1) * slope1);
            int endX = static_cast<int>(x1 + (y - y1) * slope2);

            if (y > y2)
                startX = static_cast<int>(x2 + (y - y2) * slope3);

            if (startX > endX)
                std::swap(startX, endX);

            // Clip the scanline to the image boundaries
            startX = std::max<uint32_t>(0, std::min<uint32_t>(m_Width - 1, startX));
            endX = std::max<uint32_t>(0, std::min<uint32_t>(m_Width - 1, endX));

            int currentIndex = y * m_Width + startX;
            for (int x = startX; x <= endX; x++)
            {
                // Linearly interpolate the color components
                float t = static_cast<float>(x - startX) / (endX - startX);
                uint32_t color = interpolateColor(color1, color3, t);

                // Set the pixel color in the image buffer
                m_imageBuffer[currentIndex] = color;

                currentIndex++;
            }
        }
    }

    uint32_t Renderer::interpolateColor(uint32_t color1, uint32_t color2, float t)
    {
        uint32_t a1 = (color1 >> 24) & 0xFF;
        uint32_t r1 = (color1 >> 16) & 0xFF;
        uint32_t g1 = (color1 >> 8) & 0xFF;
        uint32_t b1 = color1 & 0xFF;

        uint32_t a2 = (color2 >> 24) & 0xFF;
        uint32_t r2 = (color2 >> 16) & 0xFF;
        uint32_t g2 = (color2 >> 8) & 0xFF;
        uint32_t b2 = color2 & 0xFF;

        uint32_t a = interpolateComponent(a1, a2, t);
        uint32_t r = interpolateComponent(r1, r2, t);
        uint32_t g = interpolateComponent(g1, g2, t);
        uint32_t b = interpolateComponent(b1, b2, t);

        return (a << 24) | (r << 16) | (g << 8) | b;
    }

    uint32_t Renderer::interpolateComponent(uint32_t component1, uint32_t component2, float t)
    {
        return static_cast<uint32_t>((1.0f - t) * component1 + t * component2);
    }

    bool Renderer::isInFrontOfCamera(glm::vec3 &positionCoords, glm::vec3 &cameraDirection)
    {
        return glm::dot(positionCoords, cameraDirection) > 0.0f;
    }

    bool Renderer::isInFrontOfCamera(glm::vec3 &positionCoords1, glm::vec3 &positionCoords2, glm::vec3 &positionCoords3, glm::vec3 &cameraDirection)
    {
        return isInFrontOfCamera(positionCoords1, cameraDirection) &&
               isInFrontOfCamera(positionCoords2, cameraDirection) &&
               isInFrontOfCamera(positionCoords3, cameraDirection);
    }

    bool Renderer::isWithinImageBounds(int x, int y)
    {
        return x >= 0 && x < m_Width && y >= 0 && y < m_Height;
    }

}