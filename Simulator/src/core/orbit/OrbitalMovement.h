/**
 * @file OrbitalMovement.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief
 * @version 0.1
 * @date 2023-07-18
 *
 *
 */

/**
 * @brief Handles mean anomaly, eccentric anomaly, true anomaly, position, and velocity relationships based on orbital elements
 * 
 */

#pragma once

#include <vector>
#include <string>
#include <fstream>

#include "orbit/OrbitalObject.h"
#include "math/vectorOps.h"
#include "csv2.h"

class OrbitalMovement
{
public:
    OrbitalMovement(const OrbitalObject &orbitalObject) : m_orbitalObject(orbitalObject) {}

protected:
    OrbitalObject m_orbitalObject;

    struct KinematicParameters
    {
        double trueAnomaly;
        double radius;
        double cosTrueAnomaly;
        double sinTrueAnomaly;
        double cosArgPeriapsis;
        double sinArgPeriapsis;
        double cosLongAscNode;
        double sinLongAscNode;
        double cosInclination;
        double sinInclination;
    };

    double calculateMeanAnomaly(double timeSincePeriapsis) const;
    double calculateEccentricAnomaly(double meanAnomaly) const;
    double calculateTrueAnomaly(double eccentricAnomaly) const;

    double calculateTimeTillPeriapsis(double meanAnomaly) const;
    double calculateTimeSincePeriapsis(double meanAnomaly) const;

    double calculateNearestPeriapsisTime(double referencePeriapsisTime, double currentTime) const;

    double calculateRadius(double trueAnomaly) const;

    KinematicParameters calculateKinematicParameters(double trueAnomaly) const;
    Vector<double, 3> calculatePosition(double trueAnomaly, KinematicParameters &kinematicParameters) const;
    Vector<double, 3> calculateVelocity(double trueAnomaly, KinematicParameters &kinematicParameters) const;

    Vector<double, 3> m_position;
    Vector<double, 3> m_velocity;

};
