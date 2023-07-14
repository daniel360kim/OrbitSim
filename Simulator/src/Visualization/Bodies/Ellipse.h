/**
 * @file Ellipse.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief converts x, y, z, coordinates of the orbit into vertex and index data
 * @version 0.1
 * @date 2023-07-11
 * 
 * 
 */

#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <vector>
#include <glm/glm.hpp>

#include "../../math/vectorOps.h"
#include "../../orbit/OrbitalPropogator.h"

namespace Visualization
{
    class Ellipse : public OrbitalPropogator
    {
    public:
        Ellipse(const std::string &name, const Type &type, double mass,
                      double semiMajorAxis, double eccentricity, double inclination,
                      double longitudeOfAscendingNode, double argumentOfPeriapsis,
                      CentralBody centralBody, double timeStep);
        
        Ellipse(const OrbitalObject &orbitalObject, double timeStep);
        ~Ellipse();

        void generateVertexPositions();

        const std::vector<glm::vec3> &GetVertexPositions() const { return m_vertexPositions; }


    private:
        std::vector<glm::vec3> m_vertexPositions;
    };
}

#endif