/**
 * @file PlanetDatabase.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief 
 * @version 0.1
 * @date 2023-07-29
 * 
 * 
 */
#pragma once

#include "PlanetData.h"
#include "util/Trie.h"

#include <vector>
#include <unordered_map>
#include <string>
#include <memory>

class PlanetDatabase : public Trie<PlanetData>
{
public:
    PlanetDatabase();
    ~PlanetDatabase();

    PlanetData m_selectedPlanet; // holds the selected planet
    size_t m_selectedPlanetIndex = 0; // holds the index of the selected planet

    void load(const std::string &databasePath);

private:
    // Hash code for the planet names
    enum SSstringCode
    {
        Mercury,
        Venus,
        Earth,
        Mars,
        Jupiter,
        Saturn,
        Uranus,
        Neptune,
        Pluto,
        Invalid
    };

    bool checkIfSolarSystem(const std::string& name);
    SSstringCode hash(const std::string& name);

    std::string getTexturePath(PlanetType type);
    std::string getTexturePath(const std::string& name);
};