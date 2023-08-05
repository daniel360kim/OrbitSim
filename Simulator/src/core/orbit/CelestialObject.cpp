/**
 * @file CelestialObject.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief general class to hold information about objects in space
 * @version 0.1
 * @date 2023-06-21
 * 
 * 
 */

#include <string>
#include <sstream>
#include <iomanip>

#include "CelestialObject.h"

/**
 * @brief Construct a new Celestial Object:: Celestial Object object
 * 
 * @param name name of the object
 * @param type type of body (planet, star, satellite, moon)
 * @param mass mass of the object in kg
 */
CelestialObject::CelestialObject(const std::string& name, const Type& type, double mass)
{
    m_name = name;
    m_type = type;
    m_mass = mass;
}

std::string CelestialObject::getTypeString() const
{
    switch (m_type)
    {
    case Type::Planet:
        return "Planet";
        break;
    case Type::Exoplanet:
        return "Exoplanet";
        break;
    case Type::Star:
        return "Star";
        break;
    case Type::Satellite:
        return "Satellite";
        break;
    case Type::Moon:
        return "Moon";
        break;
    case Type::Nebulae:
        return "Nebulae";
        break;
    case Type::Cloud:
        return "Cloud";
        break;
    case Type::BlackHole:
        return "Black Hole";
        break;
    case Type::Asteroid:
        return "Asteroid";
        break;
    case Type::Comet:
        return "Comet";
        break;
    default:
        return "Unknown";
        break;
    }
    
}

std::string CelestialObject::getMassScientific(int precision) const
{
    std::stringstream ss;
    ss << std::scientific << std::setprecision(precision) << m_mass;
    return ss.str();
}
