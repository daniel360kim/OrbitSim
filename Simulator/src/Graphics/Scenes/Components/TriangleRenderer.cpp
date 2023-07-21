/**
 * @file TriangleRenderer.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief
 * @version 0.1
 * @date 2023-07-21
 *
 *
 */

#include "TriangleRenderer.h"

#include <glm/gtx/rotate_vector.hpp>

namespace Visualization
{
    void TriangleRenderer::fillTriangle(Triangle<int> indices, Triangle<uint32_t> colors, std::vector<uint32_t> &pixels)
    {
        if (indices.v2 < indices.v1)
        {
            std::swap(indices.v1, indices.v2);
            std::swap(colors.v1, colors.v2);
        }

        if (indices.v3 < indices.v2)
        {
            std::swap(indices.v2, indices.v3);
            std::swap(colors.v2, colors.v3);
        }

        if (indices.v2 < indices.v1)
        {
            std::swap(indices.v1, indices.v2);
            std::swap(colors.v1, colors.v2);
        }

        Triangle<int> xIndex;
        xIndex.v1 = indices.v1 % m_Width;
        xIndex.v2 = indices.v2 % m_Width;
        xIndex.v3 = indices.v3 % m_Width;
        
        Triangle<int> yIndex;
        yIndex.v1 = indices.v1 / m_Width;
        yIndex.v2 = indices.v2 / m_Width;
        yIndex.v3 = indices.v3 / m_Width;

        Triangle<float> slope;
        slope.v1 = static_cast<float>(xIndex.v2 - xIndex.v1) / (yIndex.v2 - yIndex.v1);
        slope.v2 = static_cast<float>(xIndex.v3 - xIndex.v1) / (yIndex.v3 - yIndex.v1);
        slope.v3 = static_cast<float>(xIndex.v3 - xIndex.v2) / (yIndex.v3 - yIndex.v2);

        // Draw scanlines
        for (int y = yIndex.v1; y <= yIndex.v3; y++)
        {
            int startX = static_cast<int>(xIndex.v1 + (y - yIndex.v1) * slope.v1);
            int endX = static_cast<int>(xIndex.v1 + (y - yIndex.v1) * slope.v2);

            if (y > yIndex.v2)
            {
                startX = static_cast<int>(xIndex.v2 + (y - yIndex.v2) * slope.v3);
            }

            if (startX > endX)
            {
                std::swap(startX, endX);
            }

            // Clip the scanlines to the image boundaries
            startX = std::max<int>(0, std::min<uint32_t>(m_Width - 1, startX));
            endX = std::max<int>(0, std::min<uint32_t>(m_Width - 1, endX));

            int currentIndex = y * m_Width + startX;
            for (int x = startX; x <= endX; x++)
            {
                // Linearly interpolate the color components
                float t = static_cast<float>(x - startX) / (endX - startX);
                uint32_t color = interpolateColor(colors.v1, colors.v3, t);

                // Set the pixel color in the image buffer
                pixels[currentIndex] = color;

                currentIndex++;
            }
        }
    }

    bool TriangleRenderer::isWithinBounds(int x, int y)
    {
        return x >= 0 && x < m_Width && y >= 0 && y < m_Height;
    }

    bool TriangleRenderer::isWithinBounds(Triangle<int> x, Triangle<int> y)
    {
        return isWithinBounds(x.v1, y.v1) && isWithinBounds(x.v2, y.v2) && isWithinBounds(x.v3, y.v3);
    }

    void TriangleRenderer::getArrayIndices(Triangle<int> &x, Triangle<int> &y, Triangle<int> &indices, Triangle<glm::vec2> pixelCoords)
    {
        x.v1 = static_cast<int>(pixelCoords.v1.x);
        x.v2 = static_cast<int>(pixelCoords.v2.x);
        x.v3 = static_cast<int>(pixelCoords.v3.x);

        y.v1 = static_cast<int>(pixelCoords.v1.y);
        y.v2 = static_cast<int>(pixelCoords.v2.y);
        y.v3 = static_cast<int>(pixelCoords.v3.y);

        indices.v1 = y.v1 * m_Width + x.v1;
        indices.v2 = y.v2 * m_Width + x.v2;
        indices.v3 = y.v3 * m_Width + x.v3;
    }

    void TriangleRenderer::applyTransformation(Triangle<glm::vec3> &trianglePositions, float yaw, float pitch, glm::vec3 position, float scale)
    {
        trianglePositions.v1 *= scale;
        trianglePositions.v2 *= scale;
        trianglePositions.v3 *= scale;

        trianglePositions.v1 = glm::rotateX(trianglePositions.v1, pitch);
        trianglePositions.v2 = glm::rotateX(trianglePositions.v2, pitch);
        trianglePositions.v3 = glm::rotateX(trianglePositions.v3, pitch);

        trianglePositions.v1 = glm::rotateY(trianglePositions.v1, yaw);
        trianglePositions.v2 = glm::rotateY(trianglePositions.v2, yaw);
        trianglePositions.v3 = glm::rotateY(trianglePositions.v3, yaw);

        trianglePositions.v1 += position;
        trianglePositions.v2 += position;
        trianglePositions.v3 += position;
    }

    bool TriangleRenderer::isInFrontOfCamera(glm::vec3 position,  glm::vec3 cameraForwardDirection)
    {
        return glm::dot(position, cameraForwardDirection) > 0.0f;
    }

    bool TriangleRenderer::isInFrontOfCamera(Triangle<glm::vec3> trianglePositions, glm::vec3 cameraForwardDirection)
    {
        return isInFrontOfCamera(trianglePositions.v1, cameraForwardDirection) &&
               isInFrontOfCamera(trianglePositions.v2, cameraForwardDirection) &&
               isInFrontOfCamera(trianglePositions.v3, cameraForwardDirection);
    }

    glm::vec2 TriangleRenderer::transformToPixelCoords(glm::vec3 positionCoords)
    {
        return glm::vec2(positionCoords.x, positionCoords.y);
    }

    void TriangleRenderer::transformToPixelCoords(Triangle<glm::vec3> trianglePositions, Triangle<glm::vec2> &trianglePixels)
    {
        trianglePixels.v1 = transformToPixelCoords(trianglePositions.v1);
        trianglePixels.v2 = transformToPixelCoords(trianglePositions.v2);
        trianglePixels.v3 = transformToPixelCoords(trianglePositions.v3);
    }

    uint32_t TriangleRenderer::colorToInt(glm::vec4 color)
    {
        uint32_t colorInt = (static_cast<uint32_t>(color.a * 255.0f) << 24) |
                            (static_cast<uint32_t>(color.b * 255.0f) << 16) |
                            (static_cast<uint32_t>(color.g * 255.0f) << 8) |
                            (static_cast<uint32_t>(color.r * 255.0f));

        return colorInt;
    }

    void TriangleRenderer::colorToInt(Triangle<glm::vec4> colors, Triangle<uint32_t> &intColors)
    {
        intColors.v1 = colorToInt(colors.v1);
        intColors.v2 = colorToInt(colors.v2);
        intColors.v3 = colorToInt(colors.v3);
    }

    uint32_t TriangleRenderer::interpolateComponent(uint32_t component1, uint32_t component2, float t)
    {
        return static_cast<uint32_t>((1.0f - t) * component1 + t * component2);
    }

    uint32_t TriangleRenderer::interpolateColor(uint32_t color1, uint32_t color2, float t)
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

}