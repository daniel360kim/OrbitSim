/**
 * @file Sphere.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief generates a sphere, texture is later mapped onto it
 * @version 0.1
 * @date 2023-07-05
 *
 *
 */

#define _USE_MATH_DEFINES
#include <cmath>

#include "Sphere.h"

namespace Visualization
{
    Sphere::Sphere(double radius, int segments, int rings)
        : m_radius(radius), m_segments(segments), m_rings(rings)
    {
        generateSphere();
    }

    void Sphere::generateSphere()
    {
        float sectorStep = 2 * M_PI / m_segments; // Angle between two vertical segments
        float stackStep = M_PI / m_rings;         // Angle between two horizontal rings

        for (int stack = 0; stack <= m_rings; ++stack)
        {
            float stackAngle = M_PI / 2 - stack * stackStep;
            float xy = m_radius * std::cosf(stackAngle);
            float z = m_radius * std::sinf(stackAngle);

            for (int sector = 0; sector <= m_segments; ++sector)
            {
                float sectorAngle = sector * sectorStep;

                float x = xy * std::cosf(sectorAngle);
                float y = xy * std::sinf(sectorAngle);

                m_positions.push_back(glm::vec3(x, y, z));               // Add vertex position
                m_normals.push_back(glm::normalize(glm::vec3(x, y, z))); // Add vertex normal
            }
        }

        for (int stack = 0; stack < m_rings; ++stack)
        {
            int k1 = stack * (m_segments + 1);
            int k2 = k1 + m_segments + 1;

            for (int sector = 0; sector < m_segments; ++sector, ++k1, ++k2)
            {
                // Form a quad using two triangles

                if (stack != 0)
                {
                    m_indices.push_back(k1); // First triangle
                    m_indices.push_back(k2);
                    m_indices.push_back(k1 + 1);
                }

                if (stack != (m_rings - 1))
                {
                    m_indices.push_back(k1 + 1); // Second triangle
                    m_indices.push_back(k2);
                    m_indices.push_back(k2 + 1);
                }
            }
        }
    }
}