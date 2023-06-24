/**
 * @file OrbitalPropogator.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief propogates time forward and predicts future positions
 * @version 0.1
 * @date 2023-06-24
 *
 *
 */

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

#include "OrbitalPropogator.h"
#include "../objects/OrbitalInvariants.h"
#include "../objects/constants.h"


double OrbitalPropogator::calculateMeanAnomaly(double timeSincePeriapsis) const
{
    return getMeanMotion() * timeSincePeriapsis;
}

double OrbitalPropogator::calculateEccentricAnomaly(double meanAnomaly) const
{
    double eccentricAnomaly = meanAnomaly;
    double error = 1e-8;
    double delta;

    do
    {
        delta = eccentricAnomaly - m_eccentricity * std::sin(eccentricAnomaly) - meanAnomaly;
        eccentricAnomaly -= delta / (1 - m_eccentricity * std::cos(eccentricAnomaly));
    } while (std::abs(delta) > error);

    return eccentricAnomaly;
}

double OrbitalPropogator::calculateTrueAnomaly(double eccentricAnomaly) const
{
    return 2 * std::atan(std::sqrt((1 + m_eccentricity) / (1 - m_eccentricity)) * std::tan(eccentricAnomaly / 2));
}

double OrbitalPropogator::calculateRadius(double trueAnomaly) const
{
    return m_semiMajorAxis * (1 - m_eccentricity * m_eccentricity) / (1 + m_eccentricity * std::cos(trueAnomaly));
}

Vector<double, 3> OrbitalPropogator::calculatePosition(double trueAnomaly) const
{
    double radius = calculateRadius(trueAnomaly);

    Vector<double, 3> position;

    position[0] = radius * (std::cos(m_argumentOfPeriapsis + trueAnomaly) * std::cos(m_longitudeOfAscendingNode) - std::sin(m_argumentOfPeriapsis + trueAnomaly) * std::sin(m_longitudeOfAscendingNode) * std::cos(m_inclination));
    position[1] = radius * (std::cos(m_argumentOfPeriapsis + trueAnomaly) * std::sin(m_longitudeOfAscendingNode) + std::sin(m_argumentOfPeriapsis + trueAnomaly) * std::cos(m_longitudeOfAscendingNode) * std::cos(m_inclination));
    position[2] = radius * (std::sin(m_argumentOfPeriapsis + trueAnomaly) * std::sin(m_inclination));

    return position;
}

Vector<double, 3> OrbitalPropogator::calculateVelocity(double trueAnomaly) const
{
    Vector<double, 3> velocity;

    OrbitalInvariants invariants(m_centralBody.getMass());
    
    //Calculate magnitude of the velocity using invariants
    double specific_energy = invariants.calculateOrbitalEnergy(m_semiMajorAxis);
    double mag_velocity = std::sqrt(2.0 * specific_energy + 2 * m_centralBody.getMass() * G / calculateRadius(trueAnomaly));

    //Calculate the velocity vector
    velocity[0] = -mag_velocity * (std::cos(m_longitudeOfAscendingNode) * std::sin(m_argumentOfPeriapsis + trueAnomaly) + std::sin(m_longitudeOfAscendingNode) * std::cos(m_argumentOfPeriapsis + trueAnomaly) * std::cos(m_inclination));
    velocity[1] = mag_velocity * (std::sin(m_longitudeOfAscendingNode) * std::sin(m_argumentOfPeriapsis + trueAnomaly) - std::cos(m_longitudeOfAscendingNode) * std::cos(m_argumentOfPeriapsis + trueAnomaly) * std::cos(m_inclination));
    velocity[2] = mag_velocity * (std::sin(m_argumentOfPeriapsis + trueAnomaly) * std::sin(m_inclination));

    return velocity;
}

void OrbitalPropogator::runTimeStep(double currentTimeStep)
{
    double meanAnomaly = calculateMeanAnomaly(currentTimeStep);
    double eccentricAnomaly = calculateEccentricAnomaly(meanAnomaly);
    m_trueAnomaly = calculateTrueAnomaly(eccentricAnomaly);

    m_position = calculatePosition(m_trueAnomaly);
    m_velocity = calculateVelocity(m_trueAnomaly);
}

void OrbitalPropogator::propogateOrbit(double duration)
{
    double currentTime = 0.0;

    while(currentTime <= duration)
    {
        runTimeStep(currentTime);

        currentTime += m_timeStep;
    }
    
}

void OrbitalPropogator::printInformation() const
{
    std::cout << "True Anomaly: " << m_trueAnomaly << "\n";
    std::cout << "Position: (" << m_position[0] << ", " << m_position[1] << ", " << m_position[2] << ")\n";
    std::cout << "Velocity: (" << m_velocity[0] << ", " << m_velocity[1] << ", " << m_velocity[2] << ")\n";
}