/**
 * @file PlanetData.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief
 * @version 0.1
 * @date 2023-07-29
 *
 *
 */

#pragma once

#include <string>

enum class PlanetType
{
    Terrestrial,
    GasGiant
};

struct PlanetData
{
    std::string name;
    int discoveryYear;
    int id;
    double mass;
    double radius;
    PlanetType type;
    std::string texturePath;
    bool isSolarSystem;
};