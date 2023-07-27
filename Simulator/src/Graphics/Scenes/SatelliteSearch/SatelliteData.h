/**
 * @file SatelliteData.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief 
 * @version 0.1
 * @date 2023-07-25
 * 
 * 
 */

#pragma once

#include <string>
#include <vector>

struct SatelliteData
{
    std::string name;
    std::string number;
    std::string classification;
    std::string launchYear;
    std::string launchNumber;
    std::string launchPiece;
    std::string epochYear;
    std::string epoch;
    int year;
    int month;
    int day;
    int hour;
    int minute;
    double inclination;
    double eccentricity;
    double argumentOfPerigee;
    double semiMajorAxis;
    double longitudeOfAscendingNode;

    bool selected = false;
};
