/**
 * @file Objects.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief Holds basic information about a celestial object (planet, star, satellite, moon)
 * @version 0.1
 * @date 2023-06-21
 * 
 * @copyright Copyright (c) 2023 OrbitOps
 * 
 */

#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>

enum class Type
{
    Planet,
    Star,
    Satellite,
    Moon,
    Nebulae,
    Cloud,
    BlackHole,
    Asteroid,
    Comet,
};

/**
 * @brief Basic information about a celestial object (planet, star, satellite, moon)
 * 
 */
class CelestialObject
{
public:
    CelestialObject(const std::string& name, const Type& type, double mass);

    double getMass() const { return m_mass; }
    std::string getName() const { return m_name; }
    Type getType() const { return m_type; }

    void setMass(const double mass) { m_mass = mass;}
    void setName(const std::string& name) { m_name = name;}
    void setType(const Type& type) { m_type = type;}

    virtual void printInformation() const = 0;

protected:
    std::string m_name;
    Type m_type;
    double m_mass;
};

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
    OrbitalObjectBuilder& setMeanAnomaly(double meanAnomaly);
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
    double m_meanAnomaly;
};

/**
 * @brief Allows for creation of a celestial object that has an orbit
 * @attention Use the builder class to create an orbital object. OrbitalObjectBuilder makes sures that all the parameters are set within range
 * 
 */
class OrbitalObject : public CelestialObject
{
public:
    OrbitalObject(const std::string& name, const Type& type, double mass,
                  double semiMajorAxis, double eccentricity, double inclination,
                  double longitudeOfAscendingNode, double argumentOfPeriapsis,
                  double meanAnomaly) : CelestialObject(name, type, mass),
                                        m_semiMajorAxis(semiMajorAxis),
                                        m_eccentricity(eccentricity),
                                        m_inclination(inclination),
                                        m_longitudeOfAscendingNode(longitudeOfAscendingNode),
                                        m_argumentOfPeriapsis(argumentOfPeriapsis),
                                        m_meanAnomaly(meanAnomaly) {}
    
    //Getters
    double getSemiMajorAxis() const { return m_semiMajorAxis; }
    double getEccentricity() const { return m_eccentricity; }
    double getInclination() const { return m_inclination; }
    double getLongitudeOfAscendingNode() const { return m_longitudeOfAscendingNode; }
    double getArgumentOfPeriapsis() const { return m_argumentOfPeriapsis; }
    double getMeanAnomaly() const { return m_meanAnomaly; }

    double getApogee() const;
    double getPerigee() const;
    double getOrbitalPeriod() const;

    //Setters
    void setSemiMajorAxis(double semiMajorAxis) { m_semiMajorAxis = semiMajorAxis; }
    void setEccentricity(double eccentricity);
    void setInclination(double inclination) { m_inclination = inclination; }
    void setLongitudeOfAscendingNode(double longitudeOfAscendingNode) { m_longitudeOfAscendingNode = longitudeOfAscendingNode; }
    void setArgumentOfPeriapsis(double argumentOfPeriapsis) { m_argumentOfPeriapsis = argumentOfPeriapsis; }
    void setMeanAnomaly(double meanAnomaly) { m_meanAnomaly = meanAnomaly; }

    void printInformation() const override;

protected:
    double m_semiMajorAxis;
    double m_eccentricity;
    double m_inclination;
    double m_longitudeOfAscendingNode;
    double m_argumentOfPeriapsis;
    double m_meanAnomaly;   
};
#endif