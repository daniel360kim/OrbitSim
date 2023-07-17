/**
 * @file Reference.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief 
 * @version 0.1
 * @date 2023-07-16
 * 
 * 
 */

#pragma once

#include "SimulationTime.h"

class Reference
{
public:
    Reference(std::string& name, SimulationTime::Time referenceTime, double degrees, double arcminutes);
    Reference(std::string& name, SimulationTime::Time referenceTime, double radians) : m_name(name), m_radians(radians), m_referenceTime(referenceTime) {}

    static double degrees_arcmin_to_radians(double degrees, double arcmin);

private:
    std::string m_name;
    double m_radians;
    SimulationTime::Time m_referenceTime;
};