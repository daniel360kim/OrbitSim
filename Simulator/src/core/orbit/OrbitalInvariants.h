/**
 * @file OrbitalInvariants.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief helper class that will eventually be used in converting geometric elements to orbital elements and vice versa
 * basically everything that changes as an object moves within its orbit
 * @version 0.1
 * @date 2023-06-21
 * 
 * 
 */

#ifndef ORBITALINVARIANTS_H
#define ORBITALINVARIANTS_H

#include "OrbitalObject.h"
#include "math/vectorOps.h"

class OrbitalInvariants
{
public:
    OrbitalInvariants(double centralBodyMass) : m_centralBodyMass(centralBodyMass) {}

    Vector<double, 3> calculateSpecificAngularMomentum(Vector<double, 3> position, Vector<double, 3> velocity) const;
    double calculateOrbitalEnergy(Vector<double, 3> position, Vector<double, 3> velocity) const;
    double calculateOrbitalEnergy(double semiMajorAxis) const;
    Vector<double, 3> calculateEccentricityVector(Vector<double, 3> position, Vector<double, 3> velocity) const;
    double calculateSemiLateratusRectum(Vector<double, 3> position, Vector<double, 3> velocity) const;

private:
    double m_centralBodyMass;
};
#endif