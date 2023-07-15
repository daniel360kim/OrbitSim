/**
 * @file OrbitalPropogator.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief propogates time forward and predicts future positions
 * @version 0.1
 * @date 2023-06-24
 *
 *
 */

#ifndef ORBITALPROPOGATOR_H
#define ORBITALPROPOGATOR_H

#include <vector>
#include <string>
#include <fstream>

#include "orbit/OrbitalObject.h"
#include "math/vectorOps.h"
#include "csv2.h"

const std::vector<std::string> headers = {"Time", "True Anomaly", "X", "Y", "Z", "VX", "VY", "VZ"};

class OrbitalPropogator
{
public:
    OrbitalPropogator(const OrbitalObject &orbitalObject) : m_orbitalObject(orbitalObject) {}
                                                                              

    void runTimeStep(double currentTimeStep);
    virtual void propogateOrbit(double duration);

    const std::vector<Vector<double, 3>> &getPositions() const { return m_positions; }

protected:
    std::vector<Vector<double, 3>> m_positions;
    OrbitalObject m_orbitalObject;
private:
    double calculateMeanAnomaly(double timeSincePeriapsis) const;
    double calculateEccentricAnomaly(double meanAnomaly) const;
    double calculateTrueAnomaly(double eccentricAnomaly) const;

    double calculateRadius(double trueAnomaly) const;

    void calculateKinematicParameters(double trueAnomaly);
    Vector<double, 3> calculatePosition(double trueAnomaly);
    Vector<double, 3> calculateVelocity(double trueAnomaly);

    double m_trueAnomaly;
    double m_radius;
    double m_cosTrueAnomaly;
    double m_sinTrueAnomaly;
    double m_cosArgPeriapsis;
    double m_sinArgPeriapsis;
    double m_cosLongAscNode;
    double m_sinLongAscNode;
    double m_cosInclination;
    double m_sinInclination;

    Vector<double, 3> m_position;
    Vector<double, 3> m_velocity; 

    std::ofstream generateLogFile() const;
    void logData(csv2::Writer<csv2::delimiter<','>> &writer, double time) const;


};

#endif