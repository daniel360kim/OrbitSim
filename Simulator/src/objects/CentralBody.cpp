/**
 * @file CentralBody.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief class for the body a satellite or other object orbits about
 * @version 0.1
 * @date 2023-06-21
 *
 * 
 */

#include <iostream>
#include <cmath>

#include "CentralBody.h"
#include "constants.h"

double CentralBody::getGravityAcceleration(double distanceFromCenter)
{
    return (G * m_mass) / (distanceFromCenter * distanceFromCenter);
}

double CentralBody::getEscapeVelocity(double distanceFromCenter) const
{
    return std::sqrt((2.0 * G * m_mass) / distanceFromCenter);
}

void CentralBody::printInformation() const
{
    std::cout << "Central Body Information for: " << m_name << std::endl;
}