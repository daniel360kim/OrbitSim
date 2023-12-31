/**
 * @file Body.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief
 * @version 0.1
 * @date 2023-07-08
 *
 *
 */

#undef max
#define _USE_MATH_DEFINES
#include <cmath>
#include <glm/gtx/rotate_vector.hpp>

#include "Bodies.h"
#include "TriangleRenderer.h"

namespace Visualization
{
    void Body::onResize(uint32_t width, uint32_t height)
    {
        m_Width = width;
        m_Height = height;
        resetCameraScaling();
    }
    void Body::Draw(CameraInfo &cameraInfo, glm::vec2 &offset, std::vector<uint32_t> &pixels)
    {
        const std::vector<unsigned int> &indices = GetIndices();
        const std::vector<glm::vec3> &positions = GetPositions();
        const std::vector<glm::vec2> &texCoords = GetTexCoords();

        TriangleRenderer triangleRenderer(m_Width, m_Height);

        for (size_t i = 0; i < indices.size(); i += 3)
        {
            Triangle<glm::vec3> trianglePosition;
            trianglePosition = {positions[indices[i + 0]], positions[indices[i + 1]], positions[indices[i + 2]]};

            // Apply transformations using the Camera properties
            glm::vec3 offset3D = glm::vec3(offset.x, offset.y, 0.0f);
            triangleRenderer.applyTransformation(trianglePosition, cameraInfo.yaw, cameraInfo.pitch, cameraInfo.position + offset3D, m_radius * m_Scaling * cameraInfo.scale);
            // Check if the positions are in front of the camera - only render what the camera can see
            Triangle<glm::vec3> cameraPositionDistance;
            cameraPositionDistance.v1 = trianglePosition.v1 - cameraInfo.position;
            cameraPositionDistance.v2 = trianglePosition.v2 - cameraInfo.position;
            cameraPositionDistance.v3 = trianglePosition.v3 - cameraInfo.position;

            if (triangleRenderer.isInFrontOfCamera(cameraPositionDistance, cameraInfo.forwardDirection))
            {
                // Transform to pixel coordinates
                Triangle<glm::vec2> pixelCoords;
                triangleRenderer.transformToPixelCoords(trianglePosition, pixelCoords);

                Triangle<int> x;
                Triangle<int> y;
                Triangle<int> index;

                triangleRenderer.getArrayIndices(x, y, index, pixelCoords);

                // Check if the pixels are within the bounds of the m_Image image
                if (triangleRenderer.isWithinBounds(x, y))
                {
                    // Map texture coordinates to the triangle vertices
                    glm::vec2 texCoords1 = texCoords[indices[i + 0]];
                    glm::vec2 texCoords2 = texCoords[indices[i + 1]];
                    glm::vec2 texCoords3 = texCoords[indices[i + 2]];

                    // Sample the texture colors
                    Triangle<glm::vec4> color;
                    color.v1 = GetTexture()->getPixel(glm::vec2(texCoords1.x, texCoords1.y));
                    color.v2 = GetTexture()->getPixel(glm::vec2(texCoords2.x, texCoords2.y));
                    color.v3 = GetTexture()->getPixel(glm::vec2(texCoords3.x, texCoords3.y));

                    // Convert the colors to 32-bit integers
                    Triangle<uint32_t> colorInt;
                    triangleRenderer.colorToInt(color, colorInt);

                    // Fill the triangle with the corresponding colors
                    triangleRenderer.fillTriangle(index, colorInt, pixels);
                }
            }
        }
    }

    glm::vec2 Body::transformToPixelCoords(glm::vec3 positionCoords, glm::vec2 &offset)
    {
        return glm::vec2(positionCoords.x, positionCoords.y) + offset;
    }

    void Body::transformToPixelCoords(Triangle<glm::vec3> &triangle,  glm::vec2 &offset, Triangle<glm::vec2> &trianglePixelCoords)
    {
        trianglePixelCoords.v1 = transformToPixelCoords(triangle.v1, offset);
        trianglePixelCoords.v2 = transformToPixelCoords(triangle.v2, offset);
        trianglePixelCoords.v3 = transformToPixelCoords(triangle.v3, offset);
    }

    uint32_t Body::convertColors(const glm::vec4 &color)
    {
        uint32_t colorInt = (static_cast<uint32_t>(color.a * 255.0f) << 24) |
                            (static_cast<uint32_t>(color.b * 255.0f) << 16) |
                            (static_cast<uint32_t>(color.g * 255.0f) << 8) |
                            (static_cast<uint32_t>(color.r * 255.0f));

        return colorInt;
    }

    void Body::fillTriangle(int index1, int index2, int index3, uint32_t color1, uint32_t color2, uint32_t color3, std::vector<uint32_t> &pixels)
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
                pixels[currentIndex] = color;

                currentIndex++;
            }
        }
    }

    uint32_t Body::interpolateColor(uint32_t color1, uint32_t color2, float t)
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

    uint32_t Body::interpolateComponent(uint32_t component1, uint32_t component2, float t)
    {
        return static_cast<uint32_t>((1.0f - t) * component1 + t * component2);
    }

    bool Body::isInFrontOfCamera(glm::vec3 &positionCoords, glm::vec3 &cameraDirection)
    {
        return glm::dot(positionCoords, cameraDirection) > 0.0f;
    }

    bool Body::isInFrontOfCamera(Triangle<glm::vec3> &positionCoords, glm::vec3 &cameraDirection)
    {
        return isInFrontOfCamera(positionCoords.v1, cameraDirection) &&
               isInFrontOfCamera(positionCoords.v2, cameraDirection) &&
               isInFrontOfCamera(positionCoords.v3, cameraDirection);
    }

    bool Body::isWithinImageBounds(int x, int y)
    {
        return x >= 0 && x < m_Width && y >= 0 && y < m_Height;
    }

    void Body::applyTransformation(Triangle<glm::vec3> &trianglePositions, float yaw, float pitch, glm::vec3 position, float scale)
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

    void Body::resetCameraScaling()
    {
        // Set initial camera scale based on the sizes of the bodies and the window size
        float maxRadius = 0.0f;

        float radius = GetRadius();
        if (radius > maxRadius)
            maxRadius = radius;

        uint32_t minDimension = std::min<float>(m_Width, m_Height);
        m_Scaling = minDimension / maxRadius / 2.0f;
    }

}