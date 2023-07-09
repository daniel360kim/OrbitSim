/**
 * @file Sphere.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief generates a sphere, texture is later mapped onto it
 * @version 0.1
 * @date 2023-07-05
 *
 *
 */

#ifndef Sphere_H
#define Sphere_H

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>
#include <array>

namespace Visualization
{
    class Sphere
    {
    public:
        Sphere(double radius, int subdivisionLevel);

        // Getters
        float GetRadius() const { return m_radius; }
        int GetSubdivisionLevel() const { return m_subdivisionLevel; }

        const std::vector<glm::vec3> &GetPositions() const { return m_positions; }
        const std::vector<glm::vec3> &GetNormals() const { return m_normals; }
        const std::vector<unsigned int> &GetIndices() const { return m_indices; }

    private:
        float m_radius;

        void subdivideTriangle(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3, int level);

    protected:
        std::vector<glm::vec3> m_positions;  // Vertex positions
        std::vector<glm::vec3> m_normals;    // Vertex normals
        std::vector<unsigned int> m_indices; // Indices for drawing sphere

        int m_subdivisionLevel;
        void generateSphere();

    
    };

}

#endif