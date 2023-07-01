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
#include <iomanip>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>

#include "OrbitalPropogator.h"
#include "../objects/OrbitalInvariants.h"
#include "../objects/constants.h"
#include "../util/FileNamer.h"
#include "csv2.h"
#include "../util/ProgressBar.h"

double OrbitalPropogator::calculateMeanAnomaly(double timeSincePeriapsis) const
{
    return m_meanMotion * timeSincePeriapsis;
}

double OrbitalPropogator::calculateEccentricAnomaly(double meanAnomaly) const
{
    const double error = 1e-8;
    const double sinEccentricAnomalyMultiplier = m_eccentricity * std::sin(meanAnomaly);
    const double cosEccentricAnomalyMultiplier = m_eccentricity * std::cos(meanAnomaly);

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

double OrbitalPropogator::calculateTrueAnomaly(double eccentricAnomaly) const
{
    return 2 * std::atan(std::sqrt((1 + m_eccentricity) / (1 - m_eccentricity)) * std::tan(eccentricAnomaly / 2));
}

double OrbitalPropogator::calculateRadius(double trueAnomaly) const
{
    return m_semiMajorAxis * (1 - m_eccentricity * m_eccentricity) / (1 + m_eccentricity * std::cos(trueAnomaly));
}

void OrbitalPropogator::calculateKinematicParameters(double trueAnomaly)
{
    m_radius = calculateRadius(trueAnomaly);
    m_cosTrueAnomaly = std::cos(trueAnomaly);
    m_sinTrueAnomaly = std::sin(trueAnomaly);
    m_cosArgPeriapsis = std::cos(m_argumentOfPeriapsis);
    m_sinArgPeriapsis = std::sin(m_argumentOfPeriapsis);
    m_cosLongAscNode = std::cos(m_longitudeOfAscendingNode);
    m_sinLongAscNode = std::sin(m_longitudeOfAscendingNode);
    m_cosInclination = std::cos(m_inclination);
    m_sinInclination = std::sin(m_inclination);
}

/**
 * @brief Calculates the position of the satellite in ECI coordinates
 *
 * @warning calculateKinematicParameters() must be called before this function
 * @param trueAnomaly true anomaly of the satellite
 * @return Vector<double, 3> x, y, z coordinates of the satellite
 */
Vector<double, 3> OrbitalPropogator::calculatePosition(double trueAnomaly)
{
    // Perifocal coordinates
    double x_per = m_radius * m_cosTrueAnomaly;
    double y_per = m_radius * m_sinTrueAnomaly;

    // Convert to ECI
    Vector<double, 3> position;
    // Perifocal to ECI
    position[0] = x_per * (-m_sinLongAscNode * m_cosInclination * m_sinArgPeriapsis + m_cosLongAscNode * m_cosArgPeriapsis) - y_per * (m_sinLongAscNode * m_cosInclination * m_cosArgPeriapsis + m_cosLongAscNode * m_sinArgPeriapsis);
    position[1] = x_per * (m_cosLongAscNode * m_cosInclination * m_sinArgPeriapsis + m_sinLongAscNode * m_cosArgPeriapsis) + y_per * (m_cosLongAscNode * m_cosInclination * m_cosArgPeriapsis - m_sinLongAscNode * m_sinArgPeriapsis);
    position[2] = x_per * (m_sinInclination * m_sinArgPeriapsis) + y_per * (m_sinInclination * m_cosArgPeriapsis);

    return position;
}

/**
 * @brief Calculates the velocity of the satellite in ECI coordinates
 *
 * @warning calculateKinematicParameters() must be called before this function
 * @param trueAnomaly true anomaly of the satellite
 * @return Vector<double, 3> x, y, z coordinates of the satellite
 */
Vector<double, 3> OrbitalPropogator::calculateVelocity(double trueAnomaly)
{
    double angular_momentum = std::sqrt(m_semiMajorAxis * m_centralBody.getGravitationalParameter() * (1.0 - m_eccentricity * m_eccentricity));
    double scale = m_centralBody.getGravitationalParameter() / angular_momentum;

    double v_x = -scale * m_sinTrueAnomaly;
    double v_y = scale * (m_eccentricity + m_cosTrueAnomaly);

    Vector<double, 3> velocity;
    velocity[0] = v_x * (-m_sinLongAscNode * m_cosInclination * m_sinArgPeriapsis + m_cosLongAscNode * m_cosArgPeriapsis) - v_y * (m_sinLongAscNode * m_cosInclination * m_cosArgPeriapsis + m_cosLongAscNode * m_sinArgPeriapsis);
    velocity[1] = v_x * (m_cosLongAscNode * m_cosInclination * m_sinArgPeriapsis + m_sinLongAscNode * m_cosArgPeriapsis) + v_y * (m_cosLongAscNode * m_cosInclination * m_cosArgPeriapsis - m_sinLongAscNode * m_sinArgPeriapsis);
    velocity[2] = v_x * (m_sinInclination * m_sinArgPeriapsis) + v_y * (m_sinInclination * m_cosArgPeriapsis);

    return velocity;
}

void OrbitalPropogator::runTimeStep(double currentTimeStep)
{
    double meanAnomaly = calculateMeanAnomaly(currentTimeStep);

    double eccentricAnomaly = calculateEccentricAnomaly(meanAnomaly);

    m_trueAnomaly = calculateTrueAnomaly(eccentricAnomaly);

    calculateKinematicParameters(m_trueAnomaly);
    m_position = calculatePosition(m_trueAnomaly);
    m_velocity = calculateVelocity(m_trueAnomaly);
}

void OrbitalPropogator::propogateOrbit(double duration)
{
    std::ofstream logFile = generateLogFile();
    csv2::Writer<csv2::delimiter<','>> csvWriter(logFile);

    double currentTime = 0.0;

    ProgressBar progressBar(20);

    while (currentTime <= duration)
    {
        runTimeStep(currentTime);
        logData(csvWriter, currentTime);

        // update progress bar every 1000 steps
        if (static_cast<int>(currentTime) % 1000 == 0)
        {
            progressBar.update(static_cast<float>(currentTime / duration));
        }

        currentTime += m_timeStep;
    }

    logFile.close();
}

void OrbitalPropogator::printInformation() const
{
    std::cout << "True Anomaly: " << m_trueAnomaly << "\n";
    std::cout << "Position: (" << m_position[0] << ", " << m_position[1] << ", " << m_position[2] << ")\n";
    std::cout << "Velocity: (" << m_velocity[0] << ", " << m_velocity[1] << ", " << m_velocity[2] << ")\n";
}

std::ofstream OrbitalPropogator::generateLogFile() const
{
    FileNamer fileNamer("../../out/prop_data.csv");

    std::string filename = fileNamer.getAvailableFilename();
    std::ofstream logFile(filename);

    std::cout << "Writing data to " << filename << "\n";

    logFile << "Time,True Anomaly,Position X,Position Y,Position Z,Velocity X,Velocity Y,Velocity Z\n";

    return logFile;
}

void OrbitalPropogator::logData(csv2::Writer<csv2::delimiter<','>> &writer, double time) const
{
    // Reserve memory for the row vector to avoid reallocations
    std::vector<std::string> row;
    row.reserve(8);

    // Convert double values to strings using a more efficient method than std::to_string
    char buffer[32]; // Sufficiently large buffer to hold the string representation
    snprintf(buffer, sizeof(buffer), "%f", time);
    row.emplace_back(buffer);

    snprintf(buffer, sizeof(buffer), "%f", m_trueAnomaly);
    row.emplace_back(buffer);

    snprintf(buffer, sizeof(buffer), "%f", m_position[0]);
    row.emplace_back(buffer);

    snprintf(buffer, sizeof(buffer), "%f", m_position[1]);
    row.emplace_back(buffer);

    snprintf(buffer, sizeof(buffer), "%f", m_position[2]);
    row.emplace_back(buffer);

    snprintf(buffer, sizeof(buffer), "%f", m_velocity[0]);
    row.emplace_back(buffer);

    snprintf(buffer, sizeof(buffer), "%f", m_velocity[1]);
    row.emplace_back(buffer);

    snprintf(buffer, sizeof(buffer), "%f", m_velocity[2]);
    row.emplace_back(buffer);

    writer.write_row(row);
}
