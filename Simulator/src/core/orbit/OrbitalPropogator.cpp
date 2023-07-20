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
#include "OrbitalInvariants.h"
#include "constants.h"
#include "util/FileNamer.h"
#include "csv2.h"
#include "util/ProgressBar.h"

OrbitalPropogator::OrbitalPropogator(const OrbitalObject &orbitalObject)
    : OrbitalMovement(orbitalObject)
{

}

void OrbitalPropogator::runTimeStep(double currentTimeStep)
{
    double meanAnomaly = calculateMeanAnomaly(currentTimeStep);

    double eccentricAnomaly = calculateEccentricAnomaly(meanAnomaly);

    double trueAnomaly = calculateTrueAnomaly(eccentricAnomaly);

    KinematicParameters kinematicParameters = calculateKinematicParameters(trueAnomaly);
    m_position = calculatePosition(trueAnomaly, kinematicParameters);
    m_velocity = calculateVelocity(trueAnomaly, kinematicParameters);
}

/**
 * @brief Propogates the orbit of the satellite for the specified duration 
 * Assumes the start time is when meanAnomaly = 0
 * Cannot be accurate in real time
 * 
 * @param duration duration to propogate the orbit for
 */
void OrbitalPropogator::propogateOrbit(double duration)
{
    std::ofstream logFile = generateLogFile();
    csv2::Writer<csv2::delimiter<','>> csvWriter(logFile);

    double currentTime = 0.0;

    // Calculate time step based on orbital period
    // larger orbital periods require larger time steps to run in a reasonable amount of time
    double timestep = m_orbitalObject.getOrbitalPeriod() / 10000.0;

    //ProgressBar progressBar(20);

    while (currentTime <= duration)
    {
        runTimeStep(currentTime);
        logData(csvWriter, currentTime);

        m_positions.push_back(m_position);

        currentTime += timestep;
    }

    logFile.close();
}


double OrbitalPropogator::getTrueAnomalyAtTime(SimulationTime::Time &time) const 
{

    double time_sec = static_cast<double>(time.dateTimeToMs()) / 1000.0;
    double timeSincePeriapsis = std::fmod(time_sec, m_orbitalObject.getOrbitalPeriod());

    double meanAnomaly = calculateMeanAnomaly(timeSincePeriapsis);
    double eccentricAnomaly = calculateEccentricAnomaly(meanAnomaly);
    double trueAnomaly = calculateTrueAnomaly(eccentricAnomaly);

    return trueAnomaly;
}


Vector<double, 3> OrbitalPropogator::getPositionAtTime(SimulationTime::Time &time) const
{
    double trueAnomaly = getTrueAnomalyAtTime(time);
    KinematicParameters kinematicParameters = calculateKinematicParameters(trueAnomaly);
    Vector<double, 3> position = calculatePosition(trueAnomaly, kinematicParameters);

    return position;
}

Vector<double, 3> OrbitalPropogator::getVelocityAtTime(SimulationTime::Time &time) const
{
    double trueAnomaly = getTrueAnomalyAtTime(time);
    KinematicParameters kinematicParameters = calculateKinematicParameters(trueAnomaly);
    Vector<double, 3> velocity = calculateVelocity(trueAnomaly, kinematicParameters);

    return velocity;
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
