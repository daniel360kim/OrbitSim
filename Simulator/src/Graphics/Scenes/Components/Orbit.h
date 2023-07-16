/**
 * @file Orbits.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief 
 * @version 0.1
 * @date 2023-07-16
 * 
 * 
 */

#pragma once

#include "orbit/OrbitalObject.h"
#include "Shapes/Ellipse.h"
#include "Bodies.h"

namespace Visualization
{   
    class Orbit : public Ellipse
    {
    public:
        Orbit(const OrbitalObject &orbitalObject) : Ellipse(orbitalObject) {}
        Orbit(const OrbitalObject &orbitalObject, Body &body);

        void setBody(const std::shared_ptr<Body> &body);
        const std::shared_ptr<Body> &GetBody() const;
    
    private:
        std::shared_ptr<Body> m_body; //for planets or moons that have orbits
        //TODO add option for satellite orbits
    };
    
} // namespace Visualization