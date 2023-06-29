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
