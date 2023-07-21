/**
 * @file Orbits.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief
 * @version 0.1
 * @date 2023-07-16
 *
 *
 */

#include "Orbit.h"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace Visualization
{
    Orbit::Orbit(const OrbitalObject &orbitalObject, uint32_t orbitColor, uint32_t iconColor, uint32_t width, uint32_t height)
        : Ellipse(orbitalObject), m_orbitColor(orbitColor), m_iconColor(iconColor), m_Width(width), m_Height(height)
    {
        m_icon = std::make_shared<Circle>(10, 5000);
    }

    void Orbit::onResize(uint32_t width, uint32_t height)
    {
        m_Width = width;
        m_Height = height;
    }

    void Orbit::DrawOrbit(CameraInfo &cameraInfo, glm::vec2 &offset, std::vector<uint32_t> &pixels, SimulationTime::Time &simulationTime)
    {
        Vector<double, 3> iconPositionRaw = getPositionAtTime(simulationTime); // get position in km
        glm::vec3 iconPosition = glm::vec3(static_cast<float>(iconPositionRaw[0]), static_cast<float>(iconPositionRaw[1]), static_cast<float>(iconPositionRaw[2]));
        int positionIndex = findClosestPositionIndex(iconPosition); // the index of the vertex positions that the icon should be on top of

        for (size_t i = 0; i < GetVertexNormals().size(); i++)
        {
            glm::vec3 position = GetVertexNormals()[i];

            position *= cameraInfo.scale * (float)GetOrbitalObject().calculateApogee(); // account for zoom, and convert back from normalized
            position *= 0.01f;                                                          // so orbit isnt huge compared to earth. TODO dynamically scale based on size of orbit
            position = glm::rotateX(position, cameraInfo.pitch);
            position = glm::rotateY(position, cameraInfo.yaw);
            position += cameraInfo.position;

            glm::vec2 pixelCoords = glm::vec2(position.x, position.y) + offset;

            int x = static_cast<int>(pixelCoords.x);
            int y = static_cast<int>(pixelCoords.y);
            int index = y * m_Width + x;

            if (i == positionIndex)
            {
                m_iconOffset = pixelCoords;
            }

            if (x >= 0 && x < m_Width && y >= 0 && y < m_Height)
            {
                pixels[index] = m_orbitColor;
            }
        }
    }
    void Orbit::DrawIcon(CameraInfo &cameraInfo, std::vector<uint32_t> &pixels)
    {
        const std::vector<glm::vec3> &vertexPositions = m_icon->GetVertexPositions();
        const std::vector<glm::vec2> &textureCoordinates = m_icon->GetTextureCoords();
        const std::vector<unsigned int> &indices = m_icon->GetIndices();

        for (size_t i = 0; i < indices.size(); i += 3)
        {
            glm::vec3 position1 = vertexPositions[indices[i + 0]];
            glm::vec3 position2 = vertexPositions[indices[i + 1]];
            glm::vec3 position3 = vertexPositions[indices[i + 2]];

            glm::vec2 pixelCoords1 = glm::vec2(position1.x, position1.y);
            glm::vec2 pixelCoords2 = glm::vec2(position2.x, position2.y);
            glm::vec2 pixelCoords3 = glm::vec2(position3.x, position3.y);

            pixelCoords1 *= m_icon->GetRadius();
            pixelCoords2 *= m_icon->GetRadius();
            pixelCoords3 *= m_icon->GetRadius();

            pixelCoords1 += m_iconOffset;
            pixelCoords2 += m_iconOffset;
            pixelCoords3 += m_iconOffset;

            int x1 = static_cast<int>(pixelCoords1.x);
            int y1 = static_cast<int>(pixelCoords1.y);
            int index1 = y1 * m_Width + x1;

            int x2 = static_cast<int>(pixelCoords2.x);
            int y2 = static_cast<int>(pixelCoords2.y);
            int index2 = y2 * m_Width + x2;

            int x3 = static_cast<int>(pixelCoords3.x);
            int y3 = static_cast<int>(pixelCoords3.y);
            int index3 = y3 * m_Width + x3;

            if (x1 >= 0 && x1 < m_Width && y1 >= 0 && y1 < m_Height &&
                x2 >= 0 && x2 < m_Width && y2 >= 0 && y2 < m_Height &&
                x3 >= 0 && x3 < m_Width && y3 >= 0 && y3 < m_Height)
            {

                fillTriangle(index1, index2, index3, m_iconColor, m_iconColor, m_iconColor, pixels);
            }
        }
    }

    // TODO this is a func used in bodies as well. make a util class
    void Orbit::fillTriangle(int index1, int index2, int index3, uint32_t color1, uint32_t color2, uint32_t color3, std::vector<uint32_t> &pixels)
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

    uint32_t Orbit::interpolateColor(uint32_t color1, uint32_t color2, float t)
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

    uint32_t Orbit::interpolateComponent(uint32_t component1, uint32_t component2, float t)
    {
        return static_cast<uint32_t>((1.0f - t) * component1 + t * component2);
    }

    size_t Orbit::findClosestPositionIndex(glm::vec3 position)
    {
        glm::vec3 closestPosition = GetVertexPositions()[0];
        float minSquaredDistance = glm::length2(position - closestPosition);

        for (size_t i = 1; i < GetVertexPositions().size(); i++)
        {
            glm::vec3 currentPosition = GetVertexPositions()[i];
            float squaredDistance = glm::length2(position - currentPosition);

            if (squaredDistance < minSquaredDistance)
            {
                closestPosition = currentPosition;
                minSquaredDistance = squaredDistance;
            }
        }

        return std::find(GetVertexPositions().begin(), GetVertexPositions().end(), closestPosition) - GetVertexPositions().begin();
    }

}