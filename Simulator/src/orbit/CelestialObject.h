/**
 * @file CelestialObject.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief general class to hold basic information of an object in space
 * @version 0.1
 * @date 2023-06-21
 * 
 * 
 */

#ifndef CELESTIALOBJECT_H
#define CELESTIALOBJECT_H

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
    CelestialObject() {}
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

#endif