/**
 * @file PlanetDatabase.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief
 * @version 0.1
 * @date 2023-07-25
 *
 *
 */

#include "PlanetDatabase.h"
#include "util/Random.h"

#include <json.hpp>
#include <fstream>
#include <iostream>
#include <cctype>

PlanetDatabase::PlanetDatabase()
{
}
PlanetDatabase::~PlanetDatabase()
{
}

void PlanetDatabase::load(const std::string &databasePath)
{
    std::ifstream file(databasePath);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open database");
    }
    nlohmann::json database;
    database = nlohmann::json::parse(file);

    if (database.is_null())
    {
        throw std::runtime_error("Failed to load database");
    }
    else if (database.is_array())
    {
        for (auto &satellite : database)
        {
            PlanetData data;
            data.name = satellite["planet_name"];
            data.discoveryYear = satellite["discovery_year"];
            data.id = satellite["id"];
            data.mass = satellite["mass"];
            data.radius = satellite["radius"];
            std::string type = satellite["planet_type"];
            if (type == "Terrestrial")
            {
                data.type = PlanetType::Terrestrial;
            }
            else if (type == "Gas Giant")
            {
                data.type = PlanetType::GasGiant;
            }
            else
            {
                throw std::runtime_error("Invalid planet type");
            }

            //If we have a SS planet we just need to provide the name of the planet to get the texture
            if (checkIfSolarSystem(data.name))
            {
                data.texturePath = getTexturePath(data.name); 
                data.isSolarSystem = true;
            }
            //If we have a non SS planet we need to provide the type of the planet to get the texture (Terrestrial or Gas Giant)
            else
            {
                data.texturePath = getTexturePath(data.type);
                data.isSolarSystem = false;
            }

            insert(data.name, data);
        }
    }
    else
    {
        throw std::runtime_error("Database is not an array");
    }
}

/**
 * @brief Checks if the planet is a SS planet that way we can map the appropriate texture to it or just apply generic texture
 *
 * @param name
 * @return true
 * @return false
 */
bool PlanetDatabase::checkIfSolarSystem(const std::string &name)
{
    switch (hash(name))
    {
    case SSstringCode::Mercury:
        return true;
    case SSstringCode::Venus:
        return true;
    case SSstringCode::Earth:
        return true;
    case SSstringCode::Mars:
        return true;
    case SSstringCode::Jupiter:
        return true;
    case SSstringCode::Saturn:
        return true;
    case SSstringCode::Uranus:
        return true;
    case SSstringCode::Neptune:
        return true;
    case SSstringCode::Pluto:
        return true;
    default:
        return false;
    }
}

/**
 * @brief Hashes the planet names so it can be used in the switch statement to check if the planet is a SS planet
 *
 * @param name
 * @return PlanetDatabase::SSstringCode
 */
PlanetDatabase::SSstringCode PlanetDatabase::hash(const std::string &name)
{
    std::string all_lowercase;
    for (auto &c : name)
    {
        all_lowercase += std::tolower(c);
    }

    if (all_lowercase == "mercury")
    {
        return SSstringCode::Mercury;
    }
    else if (all_lowercase == "venus")
    {
        return SSstringCode::Venus;
    }
    else if (all_lowercase == "earth")
    {
        return SSstringCode::Earth;
    }
    else if (all_lowercase == "mars")
    {
        return SSstringCode::Mars;
    }
    else if (all_lowercase == "jupiter")
    {
        return SSstringCode::Jupiter;
    }
    else if (all_lowercase == "saturn")
    {
        return SSstringCode::Saturn;
    }
    else if (all_lowercase == "uranus")
    {
        return SSstringCode::Uranus;
    }
    else if (all_lowercase == "neptune")
    {
        return SSstringCode::Neptune;
    }
    else if (all_lowercase == "pluto")
    {
        return SSstringCode::Pluto;
    }
    else
    {
        return SSstringCode::Invalid;
    }
}

/**
 * @brief Gets the appropriate texture path for the planet based on the planet type
 *
 * @param type
 * @param isSolarSystem
 * @return std::string
 */
std::string PlanetDatabase::getTexturePath(PlanetType type)
{

    if (type == PlanetType::Terrestrial)
    {
        //Since the exoplanets don't have textures, we will randomly choose one of the 4 textures we have for the terrestrial planets
        RandomNumber<int> textureChooser;
        int randomNum = textureChooser.get(1, 4);
        switch (randomNum)
        {
        case 1:
            return "assets/Textures/TerrestrialExoplanet1.jpg";
        case 2:
            return "assets/Textures/TerrestrialExoplanet2.jpg";
        case 3:
            return "assets/Textures/TerrestrialExoplanet3.jpg";
        case 4:
            return "assets/Textures/TerrestrialExoplanet4.jpg";
        default:
            return "assets/Textures/TerrestrialExoplanet1.jpg";
        }
    }
    else if (type == PlanetType::GasGiant)
    {
        return "assets/Textures/GaseousExoplanet.png";
    }
    else
    {
        throw std::runtime_error("Invalid planet type");
    }
}

/**
 * @brief For SS planets, this function will return the appropriate texture path for the planet.
 * 
 * @param name 
 * @return std::string 
 */
std::string PlanetDatabase::getTexturePath(const std::string &name)
{
    switch (hash(name))
    {
    case SSstringCode::Mercury:
        return "assets/Textures/mercury.jpg";
    case SSstringCode::Venus:
        return "assets/Textures/venus.jpg";
    case SSstringCode::Earth:
        return "assets/Textures/earthDay.jpg";
    case SSstringCode::Mars:
        return "assets/Textures/mars.jpg";
    case SSstringCode::Jupiter:
        return "assets/Textures/jupiter.jpg";
    case SSstringCode::Saturn:
        return "assets/Textures/saturn.jpg";
    case SSstringCode::Uranus:
        return "assets/Textures/uranus.jpg";
    case SSstringCode::Neptune:
        return "assets/Textures/neptune.jpg";
    case SSstringCode::Pluto:
        return "assets/Textures/pluto.jpg";
    default:
        throw std::runtime_error("Invalid planet name");
    }
}