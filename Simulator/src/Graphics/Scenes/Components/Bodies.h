/**
 * @file Body.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief applies a texture to a sphere
 * @version 0.1
 * @date 2023-07-08
 * 
 * 
 */

#ifndef Body_H
#define Body_H

#include "Shapes/Sphere.h"
#include "orbit/CentralBody.h"

namespace Visualization
{
    class CentralRenderBody : public Sphere
    {
    public:
        CentralRenderBody(CentralBody& centralBody, double radius, int subdivisionlevel, const std::string& texturePath)
            : Sphere(radius, subdivisionlevel, texturePath), m_centralBody(centralBody) {}
        
        CentralBody GetCentralBody() const { return m_centralBody; }

    private:
        CentralBody m_centralBody;
        
    };
}

#endif