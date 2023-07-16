/**
 * @file Sphere.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief generates a sphere, texture is later mapped onto it
 * @version 0.1
 * @date 2023-07-05
 *
 *
 */

#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>
#include <array>

#include "../Texture.h"

namespace Visualization
{
    class Sphere
    {
    public:
        Sphere(double radius, int subdivisionLevel, const std::string &texturePath);

        // Getters
        float GetRadius() const { return m_radius; }
        int GetSubdivisionLevel() const { return m_subdivisionLevel; }

        const std::vector<glm::vec3> &GetPositions() const { return m_positions; }
        const std::vector<glm::vec3> &GetNormals() const { return m_normals; }
        const std::vector<unsigned int> &GetIndices() const { return m_indices; }

        const std::vector<glm::vec2> &GetTexCoords() const { return m_texCoords; }
        std::shared_ptr<Texture> GetTexture() { return m_texture; }

        void changeSubdivisionLevel(int subdivisionLevel);
        int getSubdivisionLevel() const { return m_subdivisionLevel; }

    private:

        std::shared_ptr<Texture> m_texture;
        std::vector<glm::vec2> m_texCoords; // Texture coordinates

        void subdivideTriangle(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3, int level);
        void computeTextureCoordinates();

    protected:
        float m_radius;
        std::vector<glm::vec3> m_positions;  // Vertex positions
        std::vector<glm::vec3> m_normals;    // Vertex normals
        std::vector<unsigned int> m_indices; // Indices for drawing sphere

        int m_subdivisionLevel;
        void generateSphere();
    };

}
