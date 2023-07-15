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
#include <iomanip>
#include <stdexcept>
#include <cmath>
#include <string>

#include "OrbitalObject.h"
#include "constants.h"

/**
 * @brief Construct a new Orbital Object Builder:: Orbital Object Builder object
 * 
 * @param name name of the object
 * @param type type of body (planet, star, satellite, moon)
 * @param mass mass of the object in kg
 */
OrbitalObjectBuilder::OrbitalObjectBuilder(const std::string& name, const Type& type, double mass)
{
    m_name = name;
    m_type = type;
    m_mass = mass;
}

OrbitalObjectBuilder& OrbitalObjectBuilder::setSemiMajorAxis(double semiMajorAxis)
{
    m_semiMajorAxis = semiMajorAxis;
    return *this;
}

OrbitalObjectBuilder& OrbitalObjectBuilder::setEccentricity(double eccentricity)
{
    if(eccentricity < 0 || eccentricity >= 1)
        throw std::invalid_argument("Eccentricity must be between 0 and 1");
    m_eccentricity = eccentricity;
    return *this;
}

OrbitalObjectBuilder& OrbitalObjectBuilder::setInclination(double inclination)
{
    m_inclination = inclination;
    return *this;
}

OrbitalObjectBuilder& OrbitalObjectBuilder::setLongitudeOfAscendingNode(double longitudeOfAscendingNode)
{
    m_longitudeOfAscendingNode = longitudeOfAscendingNode;
    return *this;
}

OrbitalObjectBuilder& OrbitalObjectBuilder::setArgumentOfPeriapsis(double argumentOfPeriapsis)
{
    m_argumentOfPeriapsis = argumentOfPeriapsis;
    return *this;
}

OrbitalObjectBuilder& OrbitalObjectBuilder::setCentralBody(CentralBody centralBody)
{
    m_centralBody = centralBody;
    return *this;
}

OrbitalObject OrbitalObjectBuilder::build() const
{
    return OrbitalObject(m_name, m_type, m_mass, m_semiMajorAxis, m_eccentricity, m_inclination, m_longitudeOfAscendingNode, m_argumentOfPeriapsis, m_centralBody);
}

//*********** End of OrbitalObjectBuilder ***********//

//*********** Start of OrbitalObject ***********//

OrbitalObject::OrbitalObject(const std::string& name, const Type& type, double mass,
                  double semiMajorAxis, double eccentricity, double inclination,
                  double longitudeOfAscendingNode, double argumentOfPeriapsis,
                  CentralBody centralBody) : CelestialObject(name, type, mass),
                                        m_semiMajorAxis(semiMajorAxis),
                                        m_eccentricity(eccentricity),
                                        m_inclination(inclination),
                                        m_longitudeOfAscendingNode(longitudeOfAscendingNode),
                                        m_argumentOfPeriapsis(argumentOfPeriapsis),
                                        m_centralBody(centralBody)
{
    m_meanMotion = calculateMeanMotion();
    m_orbitalPeriod = calculateOrbitalPeriod();
}

double OrbitalObject::calculateApogee() const
{
    return m_semiMajorAxis * (1 + m_eccentricity);
}

double OrbitalObject::calculatePerigee() const
{
    return m_semiMajorAxis * (1 - m_eccentricity);
}

double OrbitalObject::calculateOrbitalPeriod() const
{
    return M_PI * 2.0 * std::sqrt(std::pow((m_semiMajorAxis * 1000.0), 3) / (G * m_centralBody.getMass()));
}

double OrbitalObject::calculateMeanMotion() const
{
    return 2.0 * M_PI / calculateOrbitalPeriod();
}

double OrbitalObject::calculateTrueAnomalyOfAscendingNode() const
{
    return 2.0 * M_PI - m_argumentOfPeriapsis;
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
    std::cout << "Apogee: " << calculateApogee() << std::endl;
    std::cout << "Perigee: " << calculatePerigee() << std::endl;
    std::cout << "Orbital period: " << std::setprecision(15) << calculateOrbitalPeriod() << std::setprecision(6) << std::endl;
}
