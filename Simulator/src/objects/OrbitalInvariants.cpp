/**
 * @file OrbitalInvariants.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief helper class that will eventually be used in converting geometric elements to orbital elements and vice versa
 * basically everything that changes as an object moves within its orbit
 * @version 0.1
 * @date 2023-06-21
 * 
 * 
 */

#include "OrbitalInvariants.h"
#include "constants.h"

#include <cmath>
#include <stdexcept>

Vector<double, 3> OrbitalInvariants::calculateSpecificAngularMomentum(Vector<double, 3> position, Vector<double, 3> velocity) const
{
    return Vector<double, 3>::cross(position, velocity);
}

double OrbitalInvariants::calculateOrbitalEnergy(Vector<double, 3> position, Vector<double, 3> velocity) const
{
    double radius = position.magnitude();
    double speed = velocity.magnitude();

    return 0.5 * (speed * speed) - (G * m_centralBodyMass) / radius;
}


double OrbitalInvariants::calculateOrbitalEnergy(double semiMajorAxis) const
{
    return -1.0 * G * m_centralBodyMass / (2.0 * semiMajorAxis);
}


Vector<double, 3> OrbitalInvariants::calculateEccentricityVector(Vector<double, 3> position, Vector<double, 3> velocity) const
{
    Vector<double, 3> specificAngularMomentum = calculateSpecificAngularMomentum(position, velocity);

    double speed = velocity.magnitude();
    double radius = position.magnitude();

    Vector<double, 3> eccentricityVector = (velocity * (speed * speed - (G * m_centralBodyMass) / radius)) - (position * Vector<double, 3>::dot(position, velocity));
    return eccentricityVector / (G * m_centralBodyMass);
}

double OrbitalInvariants::calculateSemiLateratusRectum(Vector<double, 3> position, Vector<double, 3> velocity) const
{   
    Vector<double, 3> specificAngularMomentum = calculateSpecificAngularMomentum(position, velocity);
    double h = specificAngularMomentum.magnitude();

    return h * h / (G * m_centralBodyMass);
}