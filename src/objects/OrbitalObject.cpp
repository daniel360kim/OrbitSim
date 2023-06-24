/**
 * @file OrbitalObject.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief constants for objects that orbit a centralbody
 * @version 0.1
 * @date 2023-06-21
 * 
 * 
 */

#define _USE_MATH_DEFINES

#include <iostream>
#include <stdexcept>
#include <cmath>

#include "OrbitalObject.h"
#include "constants.h"


//*********** Start of OrbitalObject ***********//

double OrbitalObject::getApogee() const
{
    return m_semiMajorAxis * (1 + m_eccentricity);
}

double OrbitalObject::getPerigee() const
{
    return m_semiMajorAxis * (1 - m_eccentricity);
}

double OrbitalObject::getOrbitalPeriod() const
{
    return M_PI * 2.0 * std::sqrt(std::pow((m_semiMajorAxis * 1000.0), 3) / (G * (m_mass + m_centralBody.getMass())));
}

void OrbitalObject::setEccentricity(double eccentricity)
{
    if(eccentricity < 0 || eccentricity >= 1)
        throw std::invalid_argument("Eccentricity must be between 0 and 1");
    m_eccentricity = eccentricity;
}

void OrbitalObject::printInformation() const
{
    std::cout << "Name: " << m_name << std::endl;
    std::cout << "Mass: " << m_mass << std::endl;

    std::cout << "Semi-major axis: " << m_semiMajorAxis << std::endl;
    std::cout << "Eccentricity: " << m_eccentricity << std::endl;
    std::cout << "Inclination: " << m_inclination << std::endl;
    std::cout << "Longitude of ascending node: " << m_longitudeOfAscendingNode << std::endl;
    std::cout << "Apogee: " << getApogee() << std::endl;
    std::cout << "Perigee: " << getPerigee() << std::endl;
    std::cout << "Orbital period: " << getOrbitalPeriod() << std::endl;
}
