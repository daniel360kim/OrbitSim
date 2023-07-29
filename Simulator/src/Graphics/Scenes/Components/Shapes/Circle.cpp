/**
 * @file Circle.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief
 * @version 0.1
 * @date 2023-07-19
 *
 *
 */


#define _USE_MATH_DEFINES
#include <cmath>

#include "Circle.h"

namespace Visualization
{
    Circle::Circle(float radius, int numVertices)
        : m_radius(radius), m_numVertices(numVertices)
    {
        generate();
    }

    void Circle::generate()
    {
        m_vertexPositions.clear();
        m_indices.clear();
        m_textureCoords.clear();

        generateVertexPositions();
        generateIndices();
        generateTextureCoords();
    }

    void Circle::generateVertexPositions()
    {
        float angleStep = 2.0f * M_PI / m_numVertices;

        for (int i = 0; i < m_numVertices; i++)
        {
            float angle = (float)i * angleStep;
            float x = m_radius * std::cos(angle);
            float y = m_radius * std::sin(angle);
            float z = 0.0f;

            glm::vec3 vertexPosition = glm::vec3(x, y, z);

            vertexPosition = glm::normalize(vertexPosition);

            m_vertexPositions.push_back(vertexPosition);
        }
    
        m_vertexPositions.push_back(glm::vec3(0.0f, 0.0f, 0.0f)); //center vertex

    }

    void Circle::generateIndices()
    {
        for (unsigned int i = 0; i < m_numVertices; i++)
        {
            m_indices.push_back(i);
            m_indices.push_back((i + 1) % m_numVertices);
            m_indices.push_back(m_numVertices); //center vertex
        }

    }

    void Circle::generateTextureCoords()
    {
        for (unsigned int i = 0; i < m_numVertices; i++)
        {
            float u = 0.5f + 0.5f * std::cos((float)i * 2.0f * M_PI / (float)m_numVertices);
            float v = 0.5f + 0.5f * std::sin((float)i * 2.0f * M_PI / (float)m_numVertices);
            m_textureCoords.push_back(glm::vec2(u, v));
        }

        m_textureCoords.push_back(glm::vec2(0.5f, 0.5f)); //center vertex
    }

}