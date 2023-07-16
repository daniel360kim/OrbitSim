/**
 * @file Orbits.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief
 * @version 0.1
 * @date 2023-07-16
 *
 *
 */

#include "Orbit.h"



namespace Visualization
{
    Orbit::Orbit(const OrbitalObject &orbitalObject, Body &body)
        : Ellipse(orbitalObject)
    {
        m_body = std::make_shared<Body>(body);
    }

    void Orbit::setBody(const std::shared_ptr<Body> &body)
    {
        m_body = body;
    }

    const std::shared_ptr<Body>& Orbit::GetBody() const
    {
        if (m_body)
        {
            return m_body;
        }
        else
        {
            throw std::runtime_error("Orbit does not have a body attached");
            return nullptr;
        }
    }

}