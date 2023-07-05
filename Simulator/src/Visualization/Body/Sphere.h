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

namespace Visualization
{
    class Sphere
    {
    public:
        Sphere(double radius, int segments, int rings);

        //Getters
        const float GetRadius() const { return m_radius; }
        const int GetSegments() const { return m_segments; }
        const int GetRings() const { return m_rings; }
        
        const std::vector<glm::vec3>& GetPositions() const { return m_positions; }
        const std::vector<glm::vec3>& GetNormals() const { return m_normals; }
        const std::vector<unsigned int>& GetIndices() const { return m_indices; }

    private:
        float m_radius;
        int m_segments; // number of segments (vertical)
        int m_rings; // number of rings (horizontal)

        std::vector<glm::vec3> m_positions; //Vertex positions
        std::vector<glm::vec3> m_normals; //Vertex normals
        std::vector<unsigned int> m_indices; //Indices for drawing sphere

        void generateSphere();
    };

}




#endif