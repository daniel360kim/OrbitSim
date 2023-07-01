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

#include "../objects/OrbitalObject.h"
#include "../math/vectorOps.h"
#include "../util/csvWriter.h"

const std::vector<std::string> headers = {"Time", "X", "Y", "Z", "VX", "VY", "VZ"};

class OrbitalPropogator : public OrbitalObject
{
public:
    OrbitalPropogator(const std::string &name, const Type &type, double mass,
                      double semiMajorAxis, double eccentricity, double inclination,
                      double longitudeOfAscendingNode, double argumentOfPeriapsis,
                      CentralBody centralBody, double timeStep) : OrbitalObject(name, type, mass, semiMajorAxis, eccentricity, inclination, longitudeOfAscendingNode, argumentOfPeriapsis, centralBody),
                                                                  m_timeStep(timeStep),
                                                                  m_csvWriter("prop_data.csv", headers) {}
                                                                  

    OrbitalPropogator(const OrbitalObject &orbitalObject, double timeStep) : OrbitalObject(orbitalObject),
                                                                             m_timeStep(timeStep),
                                                                             m_csvWriter("../../out/prop_data.csv", headers) {}

    void runTimeStep(double currentTimeStep);
    void propogateOrbit(double duration);

    void printInformation() const override;

private:
    double calculateMeanAnomaly(double timeSincePeriapsis) const;
    double calculateEccentricAnomaly(double meanAnomaly) const;
    double calculateTrueAnomaly(double eccentricAnomaly) const;

    double calculateRadius(double trueAnomaly) const;

    Vector<double, 3> calculatePosition(double trueAnomaly) const;
    Vector<double, 3> calculateVelocity(double trueAnomaly) const;
    double m_timeStep;

    double m_trueAnomaly;
    Vector<double, 3> m_position;
    Vector<double, 3> m_velocity;

    CSVWriter<double, double, double, double, double, double, double> m_csvWriter;

    void writeDataToFile(double currentTime);

};

#endif