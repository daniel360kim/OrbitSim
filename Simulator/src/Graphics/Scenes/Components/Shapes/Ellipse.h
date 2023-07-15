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

#include "math/vectorOps.h"
#include "../OrbitalPropogator.h"

namespace Visualization
{
    class Ellipse : public OrbitalPropogator
    {
    public:
        Ellipse(const OrbitalObject &orbitalObject);
        ~Ellipse();

        const std::vector<glm::vec3> &GetVertexPositions() const { return m_vertexPositions; }

    private:
        std::vector<glm::vec3> m_vertexPositions;
        void generateVertexPositions();
    };
}

#endif