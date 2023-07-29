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
#include "TriangleRenderer.h"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace Visualization
{
    Orbit::Orbit(const OrbitalObject &orbitalObject, glm::vec4 orbitColor, glm::vec4 iconColor, uint32_t width, uint32_t height)
        : Ellipse(orbitalObject), m_orbitColor(orbitColor), m_iconColor(iconColor), m_Width(width), m_Height(height)
    {
        m_icon = std::make_shared<Circle>(10.0f, 5000);
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
            float positionScaleFactor = 1.0f / std::log10f(float(GetOrbitalObject().calculateApogee())); // scale factor for position so orbit isnt huge compared to earth. 
            position *= positionScaleFactor;                                                
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
                pixels[index] = ColorConversion::colorToInt(m_orbitColor);
            }
        }
    }
    void Orbit::DrawIcon(CameraInfo &cameraInfo, std::vector<uint32_t> &pixels)
    {
        const std::vector<glm::vec3> &vertexPositions = m_icon->GetVertexPositions();
        const std::vector<glm::vec2> &textureCoordinates = m_icon->GetTextureCoords();
        const std::vector<unsigned int> &indices = m_icon->GetIndices();

        TriangleRenderer triangleRenderer(m_Width, m_Height);

        for (size_t i = 0; i < indices.size(); i += 3)
        {
            Triangle<glm::vec3> trianglePosition = {vertexPositions[indices[i + 0]], vertexPositions[indices[i + 1]], vertexPositions[indices[i + 2]]};

            // m_iconOffset is transformed already so no need to do it again

            Triangle<glm::vec2> pixelCoords;
            triangleRenderer.transformToPixelCoords(trianglePosition, pixelCoords);

            pixelCoords.v1 *= m_icon->GetRadius();
            pixelCoords.v2 *= m_icon->GetRadius();
            pixelCoords.v3 *= m_icon->GetRadius();

            pixelCoords.v1 += m_iconOffset;
            pixelCoords.v2 += m_iconOffset;
            pixelCoords.v3 += m_iconOffset;


            Triangle<int> x;
            Triangle<int> y;
            Triangle<int> index;

            triangleRenderer.getArrayIndices(x, y, index, pixelCoords);

            if (triangleRenderer.isWithinBounds(x, y))
            {
                Triangle<uint32_t> color;
                color.v1 = ColorConversion::colorToInt(m_iconColor);
                color.v2 = ColorConversion::colorToInt(m_iconColor);
                color.v3 = ColorConversion::colorToInt(m_iconColor);

                triangleRenderer.fillTriangle(index, color, pixels);
            }
        }
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