/**
 * @file OrbitalObject.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief constants for objects that orbit a centralbody
 * @version 0.1
 * @date 2023-06-21
 * 
 * 
 */

#pragma once

#include "CentralBody.h"
#include "CelestialObject.h"
#include "math/vectorOps.h"
#include "OrbitalInvariants.h"

class OrbitalObject; // forward declaration

/**
 * @brief Use this to build and set the orbital parameters of an object
 * 
 */
class OrbitalObjectBuilder
{
public:
    OrbitalObjectBuilder(const std::string &name, const Type &type, double mass);
    OrbitalObjectBuilder& setSemiMajorAxis(double semiMajorAxis);
    OrbitalObjectBuilder& setEccentricity(double eccentricity);
    OrbitalObjectBuilder& setInclination(double inclination);
    OrbitalObjectBuilder& setLongitudeOfAscendingNode(double longitudeOfAscendingNode);
    OrbitalObjectBuilder& setArgumentOfPeriapsis(double argumentOfPeriapsis);
    OrbitalObjectBuilder& setCentralBody(CentralBody centralBody);
    OrbitalObject build() const;

private:
    std::string m_name;
    Type m_type;
    double m_mass;
    double m_semiMajorAxis;
    double m_eccentricity;
    double m_inclination;
    double m_longitudeOfAscendingNode;
    double m_argumentOfPeriapsis;
    CentralBody m_centralBody;
};

/**
 * @brief Allows for creation of a celestial object that is in orbit about a central body
 * @attention Use the builder class to create an orbital object. OrbitalObjectBuilder makes sures that all the parameters are set within range
 * 
 */
class OrbitalObject : public CelestialObject
{
public:
    OrbitalObject() {}
    OrbitalObject(const std::string& name, const Type& type, double mass,
                  double semiMajorAxis, double eccentricity, double inclination,
                  double longitudeOfAscendingNode, double argumentOfPeriapsis,
                  CentralBody centralBody);

    //Getters
    double getSemiMajorAxis() const { return m_semiMajorAxis; }
    double getEccentricity() const { return m_eccentricity; }
    double getInclination() const { return m_inclination; }
    double getLongitudeOfAscendingNode() const { return m_longitudeOfAscendingNode; }
    double getArgumentOfPeriapsis() const { return m_argumentOfPeriapsis; }
    double getOrbitalPeriod() const { return m_orbitalPeriod; }
    double getMeanMotion() const { return m_meanMotion; }
    CentralBody getCentralBody() const { return m_centralBody; }

    double calculateApogee() const;
    double calculatePerigee() const;
    double calculateTrueAnomalyOfAscendingNode() const;
    double calculateOrbitalPeriod() const;
    double calculateMeanMotion() const;

    //Setters
    void setSemiMajorAxis(double semiMajorAxis) { m_semiMajorAxis = semiMajorAxis; }
    void setEccentricity(double eccentricity);
    void setInclination(double inclination) { m_inclination = inclination; }
    void setLongitudeOfAscendingNode(double longitudeOfAscendingNode) { m_longitudeOfAscendingNode = longitudeOfAscendingNode; }
    void setArgumentOfPeriapsis(double argumentOfPeriapsis) { m_argumentOfPeriapsis = argumentOfPeriapsis; }
    void printInformation() const override;

private:
    double m_semiMajorAxis;
    double m_eccentricity;
    double m_inclination;
    double m_longitudeOfAscendingNode;
    double m_argumentOfPeriapsis;
    double m_meanMotion;
    double m_orbitalPeriod;
    CentralBody m_centralBody;

};
