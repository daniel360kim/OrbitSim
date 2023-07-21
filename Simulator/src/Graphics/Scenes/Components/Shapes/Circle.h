/**
 * @file Circle.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief 
 * @version 0.1
 * @date 2023-07-19
 * 
 * 
 */

#pragma once

#include <vector>
#include <string>
#include <memory>
#include <glm/glm.hpp>

#include "../Texture.h"

namespace Visualization
{
    class Circle
    {
    public:
        Circle(float radius, int numVertices);
        void generate();

        const std::vector<glm::vec3> &GetVertexPositions() const { return m_vertexPositions; }
        const std::vector<unsigned int> &GetIndices() const { return m_indices; }
        const std::vector<glm::vec2> &GetTextureCoords() const { return m_textureCoords; }

        float GetRadius() const { return m_radius; }

    private:
        float m_radius;
        int m_numVertices;

        std::vector<glm::vec3> m_vertexPositions;
        std::vector<unsigned int> m_indices;
        std::vector<glm::vec2> m_textureCoords;

        void generateVertexPositions();
        void generateIndices();
        void generateTextureCoords();
    };
}