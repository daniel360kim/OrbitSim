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
#include <iostream>

#include "Sphere.h"

namespace Visualization
{
    Sphere::Sphere(double radius, int subdivisionLevel, const std::string &texturePath)
        : m_radius(radius), m_subdivisionLevel(subdivisionLevel)
    {
        m_texture = std::make_shared<Texture>(texturePath);
        generateSphere();
        computeTextureCoordinates();
    }

    void Sphere::generateSphere()
    {
        m_positions.clear();
        m_normals.clear();
        m_indices.clear();
        
        // Create an icosahedron (a 20-sided polyhedron) as the base mesh
        const float t = (1.0f + std::sqrt(5.0f)) / 2.0f;

        std::vector<glm::vec3> vertices = {
            glm::normalize(glm::vec3(-1, t, 0)),
            glm::normalize(glm::vec3(1, t, 0)),
            glm::normalize(glm::vec3(-1, -t, 0)),
            glm::normalize(glm::vec3(1, -t, 0)),

            glm::normalize(glm::vec3(0, -1, t)),
            glm::normalize(glm::vec3(0, 1, t)),
            glm::normalize(glm::vec3(0, -1, -t)),
            glm::normalize(glm::vec3(0, 1, -t)),

            glm::normalize(glm::vec3(t, 0, -1)),
            glm::normalize(glm::vec3(t, 0, 1)),
            glm::normalize(glm::vec3(-t, 0, -1)),
            glm::normalize(glm::vec3(-t, 0, 1))};

        std::vector<unsigned int> indices = {
            0, 11, 5, 0, 5, 1, 0, 1, 7, 0, 7, 10, 0, 10, 11,
            1, 5, 9, 5, 11, 4, 11, 10, 2, 10, 7, 6, 7, 1, 8,
            3, 9, 4, 3, 4, 2, 3, 2, 6, 3, 6, 8, 3, 8, 9,
            4, 9, 5, 2, 4, 11, 6, 2, 10, 8, 6, 7, 9, 8, 1};

        // Subdivide the triangles to increase the mesh resolution
        for (unsigned int i = 0; i < indices.size(); i += 3)
        {
            glm::vec3 v1 = vertices[indices[i]];
            glm::vec3 v2 = vertices[indices[i + 1]];
            glm::vec3 v3 = vertices[indices[i + 2]];

            subdivideTriangle(v1, v2, v3, m_subdivisionLevel);
        }

        // Normalize all the vertex positions to make the sphere radius equal to 1
        for (glm::vec3 &position : m_positions)
        {
            position = glm::normalize(position);
        }

        // Set the normals based on the vertex positions
        m_normals = m_positions;

        // Set the indices for rendering
        m_indices.resize(m_positions.size());
        for (unsigned int i = 0; i < m_positions.size(); i++)
        {
            m_indices[i] = i;
        }
    }

    void Sphere::subdivideTriangle(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3, int level)
    {
        if (level == 0)
        {
            m_positions.push_back(v1);
            m_positions.push_back(v2);
            m_positions.push_back(v3);
            return;
        }

        glm::vec3 v12 = glm::normalize(v1 + v2);
        glm::vec3 v23 = glm::normalize(v2 + v3);
        glm::vec3 v31 = glm::normalize(v3 + v1);

        subdivideTriangle(v1, v12, v31, level - 1);
        subdivideTriangle(v2, v23, v12, level - 1);
        subdivideTriangle(v3, v31, v23, level - 1);
        subdivideTriangle(v12, v23, v31, level - 1);
    }

    void Sphere::computeTextureCoordinates()
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

    void Sphere::changeSubdivisionLevel(int subdivisionLevel)
    {
        if (subdivisionLevel < 0)
            return;
        else if (subdivisionLevel == m_subdivisionLevel)
            return;
        m_subdivisionLevel = subdivisionLevel;
        generateSphere();
        computeTextureCoordinates();
    }

}