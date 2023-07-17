/**
 * @file Reference.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief
 * @version 0.1
 * @date 2023-07-16
 *
 *
 */

#define _USE_MATH_DEFINES
#include <cmath>

#include "Reference.h"

Reference::Reference(std::string& name, SimulationTime::Time referenceTime, double degrees, double arcminutes)
    : m_name(name), m_referenceTime(referenceTime)
{
    m_radians = degrees_arcmin_to_radians(degrees, arcminutes);
}


double Reference::degrees_arcmin_to_radians(double degrees, double arcmin)
{
    return (degrees + arcmin / 60.0) * M_PI / 180.0;
}