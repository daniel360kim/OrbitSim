/**
 * @file Ellipse.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief
 * @version 0.1
 * @date 2023-07-11
 *
 *
 */

#define _USE_MATH_DEFINES
#include <cmath>

#include "Ellipse.h"

namespace Visualization
{
    Ellipse::Ellipse(const OrbitalObject &orbitalObject)
        : OrbitalPropogator(orbitalObject), m_orbitalObject(orbitalObject)
    {
        propogateOrbit(m_orbitalObject.getOrbitalPeriod());
        generateVertexPositions();
    }

    Ellipse::~Ellipse()
    {
    }


    /**
     * @brief Converts the coordinates, in km, to vertex positions in screen space
     * 
     */
    void Ellipse::generateVertexPositions()
    {
        for (auto &coord : m_positions)
        {
            float x = static_cast<float>(coord[0]);
            float y = static_cast<float>(coord[1]);
            float z = static_cast<float>(coord[2]);
            
            glm::vec3 vertexPosition = glm::vec3(x, y, z);
            
            vertexPosition = glm::normalize(vertexPosition);
            m_vertexPositions.push_back(vertexPosition);


        }
    }

}