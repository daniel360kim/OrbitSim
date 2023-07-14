/**
 * @file Body.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief 
 * @version 0.1
 * @date 2023-07-08
 * 
 * 
 */

#define _USE_MATH_DEFINES
#include <cmath>

#include "Body.h"

namespace Visualization
{
    Body::Body(float radius, int subdivisionLevel, const std::string &texturePath, BodyType type)
        : Sphere(radius, subdivisionLevel), m_type(type)
    {
        m_texture = std::make_shared<Texture>(texturePath);
        computeTextureCoordinates();
    }

    Body::~Body()
    {
    }

    void Body::computeTextureCoordinates()
    {
        m_texCoords.resize(m_positions.size());

        for (unsigned int i = 0; i < m_positions.size(); i++)
        {
            glm::vec3 position = m_positions[i];

            float u = 0.5f + std::atan2(position.z, position.x) / (2 * M_PI);
            float v = 0.5f - std::asin(position.y) / M_PI;

            // Normalize the texture coordinates
            u = std::fmod(u, 1.0f);
            v = std::fmod(v, 1.0f);

            if (u < 0.0f)
                u += 1.0f;
            if (v < 0.0f)
                v += 1.0f;

            m_texCoords[i] = glm::vec2(u, v);
        }
    }

    void Body::changeSubdivisionLevel(int subdivisionLevel)
    {
        m_subdivisionLevel = subdivisionLevel;
        generateSphere();
        computeTextureCoordinates();
    }
}