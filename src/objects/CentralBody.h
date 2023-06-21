/**
 * @file CentralBody.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief class for the body a satellite or other object orbits about
 * @version 0.1
 * @date 2023-06-21
 *
 * 
 */

#ifndef CENTRALBODY_H
#define CENTRALBODY_H

#include <string>

#include "CelestialObject.h"

/**
 * @brief Central body that satellites orbit. Assumes its a sphere 
 * Additional shapes may come soon, however, shape doesn't affect orbit that much
 * 
 */
class CentralBody : public CelestialObject
{
public:
    CentralBody() {}
    CentralBody(const std::string &name, const Type& type, double mass, double radius, double rotationalPeriod) 
                : CelestialObject(name, type, mass), m_radius(radius), m_rotationPeriod(rotationalPeriod) {}

    
    //getters
    double getRadius() const { return m_radius; }
    double getRotationalPeriod() const { return m_rotationPeriod; }
    double getGravityAcceleration(double distanceFromCenter);
    double getEscapeVelocity(double distanceFromCenter) const;

    void printInformation() const override;

private:
    double m_radius;
    double m_rotationPeriod;
};





















#endif