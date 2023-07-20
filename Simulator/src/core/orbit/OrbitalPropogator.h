/**
 * @file OrbitalPropogator.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief propogates time forward and predicts future positions
 * @version 0.1
 * @date 2023-06-24
 *
 *
 */

#pragma once

#include "OrbitalMovement.h"
#include "SimulationTime.h"

const std::vector<std::string> headers = {"Time", "X", "Y", "Z", "VX", "VY", "VZ"};

class OrbitalPropogator : public OrbitalMovement
{
public:
    OrbitalPropogator(const OrbitalObject &orbitalObject);

    void runTimeStep(double currentTimeStep);
    void propogateOrbit(double duration);

    double getTrueAnomalyAtTime(SimulationTime::Time& time) const;

    Vector<double, 3> getPositionAtTime(SimulationTime::Time& time) const;
    Vector<double, 3> getVelocityAtTime(SimulationTime::Time& time) const;

    const std::vector<Vector<double, 3>> &getPositions() const { return m_positions; }

protected:
    std::vector<Vector<double, 3>> m_positions;

    Vector<double, 3> m_position;
    Vector<double, 3> m_velocity;

private:
    std::ofstream generateLogFile() const;
    void logData(csv2::Writer<csv2::delimiter<','>> &writer, double time) const;
};
