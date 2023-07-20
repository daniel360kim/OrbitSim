/**
 * @file OrbitalMovement.cpp
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
#include <iomanip>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>

#include "OrbitalMovement.h"
#include "OrbitalInvariants.h"
#include "constants.h"
#include "util/FileNamer.h"
#include "csv2.h"

double OrbitalMovement::calculateMeanAnomaly(double timeSincePeriapsis) const
{
    double meanMotion = m_orbitalObject.getMeanMotion();
    return meanMotion * timeSincePeriapsis;
}

double OrbitalMovement::calculateEccentricAnomaly(double meanAnomaly) const
{
    const double error = 1e-8;
    double eccentricity = m_orbitalObject.getEccentricity();
    const double sinEccentricAnomalyMultiplier = eccentricity * std::sin(meanAnomaly);
    const double cosEccentricAnomalyMultiplier = eccentricity * std::cos(meanAnomaly);

    double eccentricAnomaly = meanAnomaly + sinEccentricAnomalyMultiplier;

    double delta;
    double denominator;

    do
    {
        delta = eccentricAnomaly - sinEccentricAnomalyMultiplier - meanAnomaly;
        denominator = 1.0 - cosEccentricAnomalyMultiplier;

        eccentricAnomaly -= delta / denominator;
    } while (std::abs(delta) > error * std::abs(meanAnomaly));

    return eccentricAnomaly;
}

double OrbitalMovement::calculateTrueAnomaly(double eccentricAnomaly) const
{
    return 2 * std::atan(std::sqrt((1 + m_orbitalObject.getEccentricity()) / (1 - m_orbitalObject.getEccentricity())) * std::tan(eccentricAnomaly / 2));
}

double OrbitalMovement::calculateTimeTillPeriapsis(double meanAnomaly) const
{
    double timeSincePeriapsis = calculateTimeSincePeriapsis(meanAnomaly);
    return m_orbitalObject.getOrbitalPeriod() - timeSincePeriapsis;
}

double OrbitalMovement::calculateTimeSincePeriapsis(double meanAnomaly) const
{
    double meanMotion = m_orbitalObject.getMeanMotion();
    return meanAnomaly / meanMotion;
}

double OrbitalMovement::calculateRadius(double trueAnomaly) const
{
    double eccentricity = m_orbitalObject.getEccentricity();
    return m_orbitalObject.getSemiMajorAxis() * (1 - eccentricity * eccentricity) / (1 + eccentricity * std::cos(trueAnomaly));
}

/**
 * @brief Calculates the closest periapsis time before the current time using a known periapsis time
 * 
 * @param referencePeriapsisTime known time of periapsis
 * @param currentTime current time to calculate the closest periapsis time before
 * @return unsigned long long closest periapsis time before the current time in seconds
 */
double OrbitalMovement::calculateNearestPeriapsisTime(double referencePeriapsisTime, double currentTime) const
{
    double time = referencePeriapsisTime;

    while(time < currentTime)
    {
        time += m_orbitalObject.getOrbitalPeriod();
    }

    return time - m_orbitalObject.getOrbitalPeriod();
}

OrbitalMovement::KinematicParameters OrbitalMovement::calculateKinematicParameters(double trueAnomaly) const
{
    KinematicParameters kinematicParameters;

    kinematicParameters.radius = calculateRadius(trueAnomaly);
    kinematicParameters.cosTrueAnomaly = std::cos(trueAnomaly);
    kinematicParameters.sinTrueAnomaly = std::sin(trueAnomaly);
    kinematicParameters.cosArgPeriapsis = std::cos(m_orbitalObject.getArgumentOfPeriapsis());
    kinematicParameters.sinArgPeriapsis = std::sin(m_orbitalObject.getArgumentOfPeriapsis());
    kinematicParameters.cosLongAscNode = std::cos(m_orbitalObject.getLongitudeOfAscendingNode());
    kinematicParameters.sinLongAscNode = std::sin(m_orbitalObject.getLongitudeOfAscendingNode());
    kinematicParameters.cosInclination = std::cos(m_orbitalObject.getInclination());
    kinematicParameters.sinInclination = std::sin(m_orbitalObject.getInclination());

    return kinematicParameters;
}

/**
 * @brief Calculates the position of the satellite in ECI coordinates
 *
 * @warning calculateKinematicParameters() must be called before this function
 * @param trueAnomaly true anomaly of the satellite
 * @return Vector<double, 3> x, y, z coordinates of the satellite
 */
Vector<double, 3> OrbitalMovement::calculatePosition(double trueAnomaly, KinematicParameters& kinematicParameters) const
{
    // Perifocal coordinates
    double x_per = kinematicParameters.radius * kinematicParameters.cosTrueAnomaly;
    double y_per = kinematicParameters.radius * kinematicParameters.sinTrueAnomaly;

    // Convert to ECI
    Vector<double, 3> position;
    // Perifocal to ECI
    position[0] = x_per * (-kinematicParameters.sinLongAscNode * kinematicParameters.cosInclination * kinematicParameters.sinArgPeriapsis + kinematicParameters.cosLongAscNode * kinematicParameters.cosArgPeriapsis) - y_per 
                        * (kinematicParameters.sinLongAscNode * kinematicParameters.cosInclination * kinematicParameters.cosArgPeriapsis + kinematicParameters.cosLongAscNode * kinematicParameters.sinArgPeriapsis);
    position[1] = x_per * (kinematicParameters.cosLongAscNode * kinematicParameters.cosInclination * kinematicParameters.sinArgPeriapsis + kinematicParameters.sinLongAscNode * kinematicParameters.cosArgPeriapsis) + y_per 
                        * (kinematicParameters.cosLongAscNode * kinematicParameters.cosInclination * kinematicParameters.cosArgPeriapsis - kinematicParameters.sinLongAscNode * kinematicParameters.sinArgPeriapsis);
    position[2] = x_per * (kinematicParameters.sinInclination * kinematicParameters.sinArgPeriapsis) + y_per * (kinematicParameters.sinInclination * kinematicParameters.cosArgPeriapsis);

    return position;
}

/**
 * @brief Calculates the velocity of the satellite in ECI coordinates
 *
 * @warning calculateKinematicParameters() must be called before this function
 * @param trueAnomaly true anomaly of the satellite
 * @return Vector<double, 3> x, y, z coordinates of the satellite
 */
Vector<double, 3> OrbitalMovement::calculateVelocity(double trueAnomaly, KinematicParameters& kinematicParameters) const
{
    double angular_momentum = std::sqrt(m_orbitalObject.getSemiMajorAxis() * m_orbitalObject.getCentralBody().getGravitationalParameter() * (1.0 - m_orbitalObject.getEccentricity() * m_orbitalObject.getEccentricity()));
    double scale = m_orbitalObject.getCentralBody().getGravitationalParameter() / angular_momentum;

    double v_x = -scale * kinematicParameters.sinTrueAnomaly;
    double v_y = scale * (m_orbitalObject.getEccentricity() + kinematicParameters.cosTrueAnomaly);

    Vector<double, 3> velocity;
    velocity[0] = v_x * (-kinematicParameters.sinLongAscNode * kinematicParameters.cosInclination * kinematicParameters.sinArgPeriapsis + kinematicParameters.cosLongAscNode * kinematicParameters.cosArgPeriapsis) - v_y 
                      * (kinematicParameters.sinLongAscNode * kinematicParameters.cosInclination * kinematicParameters.cosArgPeriapsis + kinematicParameters.cosLongAscNode * kinematicParameters.sinArgPeriapsis);
    velocity[1] = v_x * (kinematicParameters.cosLongAscNode * kinematicParameters.cosInclination * kinematicParameters.sinArgPeriapsis + kinematicParameters.sinLongAscNode * kinematicParameters.cosArgPeriapsis) + v_y 
                      * (kinematicParameters.cosLongAscNode * kinematicParameters.cosInclination * kinematicParameters.cosArgPeriapsis - kinematicParameters.sinLongAscNode * kinematicParameters.sinArgPeriapsis);
    velocity[2] = v_x * (kinematicParameters.sinInclination * kinematicParameters.sinArgPeriapsis) + v_y * (kinematicParameters.sinInclination * kinematicParameters.cosArgPeriapsis);

    return velocity;
}




