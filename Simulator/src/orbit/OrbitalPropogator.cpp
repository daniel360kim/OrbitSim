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

Vector<double, 3> OrbitalPropogator::calculatePosition(double trueAnomaly) const
{
    const double radius = calculateRadius(trueAnomaly);
    const double cosTrueAnomaly = std::cos(trueAnomaly);
    const double sinTrueAnomaly = std::sin(trueAnomaly);
    const double cosArgPeriapsis = std::cos(m_argumentOfPeriapsis);
    const double sinArgPeriapsis = std::sin(m_argumentOfPeriapsis);
    const double cosLongAscNode = std::cos(m_longitudeOfAscendingNode);
    const double sinLongAscNode = std::sin(m_longitudeOfAscendingNode);
    const double cosInclination = std::cos(m_inclination);
    const double sinInclination = std::sin(m_inclination);

    Vector<double, 3> position;

    position[0] = radius * (cosArgPeriapsis * cosTrueAnomaly * cosLongAscNode - sinArgPeriapsis * sinTrueAnomaly * sinLongAscNode * cosInclination);
    position[1] = radius * (cosArgPeriapsis * cosTrueAnomaly * sinLongAscNode + sinArgPeriapsis * sinTrueAnomaly * cosLongAscNode * cosInclination);
    position[2] = radius * (sinArgPeriapsis * cosTrueAnomaly * sinInclination);

    return position;
}


Vector<double, 3> OrbitalPropogator::calculateVelocity(double trueAnomaly) const
{
    Vector<double, 3> velocity;

    OrbitalInvariants invariants(m_centralBody.getMass());

    const double cosLongAscNode = std::cos(m_longitudeOfAscendingNode);
    const double sinArgPeriapsisTrueAnomaly = std::sin(m_argumentOfPeriapsis + trueAnomaly);
    const double cosArgPeriapsisTrueAnomaly = std::cos(m_argumentOfPeriapsis + trueAnomaly);
    const double cosInclination = std::cos(m_inclination);
    const double sinInclination = std::sin(m_inclination);

    // Calculate magnitude of the velocity using invariants
    double specific_energy = invariants.calculateOrbitalEnergy(m_semiMajorAxis);
    double mag_velocity = std::sqrt(2.0 * specific_energy + 2 * m_centralBody.getGravitationalParameter() / calculateRadius(trueAnomaly));

    // Calculate the velocity vector
    velocity[0] = -mag_velocity * (cosLongAscNode * sinArgPeriapsisTrueAnomaly + std::sin(m_longitudeOfAscendingNode) * cosArgPeriapsisTrueAnomaly * cosInclination);
    velocity[1] = mag_velocity * (std::sin(m_longitudeOfAscendingNode) * sinArgPeriapsisTrueAnomaly - cosLongAscNode * cosArgPeriapsisTrueAnomaly * cosInclination);
    velocity[2] = mag_velocity * (sinArgPeriapsisTrueAnomaly * sinInclination);

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
    std::ofstream logFile = generateLogFile();
    csv2::Writer<csv2::delimiter<','>> csvWriter(logFile);

    double currentTime = 0.0;
    double averageTimeStep = 0.0;

    ProgressBar progressBar(20);

    while(currentTime <= duration)
    {
        runTimeStep(currentTime);
        //progressBar.update(static_cast<float>(currentTime / duration));

        auto start = std::chrono::high_resolution_clock::now();
        logData(csvWriter, currentTime);
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        averageTimeStep += elapsed.count();

        //update progress bar every 1000 steps
        if (static_cast<int>(currentTime) % 1000 == 0)
        {
            progressBar.update(static_cast<float>(currentTime / duration));
        }
        
        currentTime += m_timeStep;
    }

    std::cout << "\nAverage Time Step: " << averageTimeStep / duration << "\n";
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
    std::ofstream logFile(fileNamer.getAvailableFilename());

    logFile << "Time,True Anomaly,Position X,Position Y,Position Z,Velocity X,Velocity Y,Velocity Z\n";

    return logFile;
}

void OrbitalPropogator::logData(csv2::Writer<csv2::delimiter<','>>& writer, double time) const
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
