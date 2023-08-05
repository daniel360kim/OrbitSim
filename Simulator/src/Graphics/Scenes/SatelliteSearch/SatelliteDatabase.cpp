/**
 * @file SatelliteDatabase.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief
 * @version 0.1
 * @date 2023-07-25
 *
 *
 */

#include "SatelliteDatabase.h"

#include <json.hpp>
#include <fstream>
#include <iostream>
#include <cctype>

SatelliteDatabase::SatelliteDatabase()
{
}
SatelliteDatabase::~SatelliteDatabase()
{

}

void SatelliteDatabase::load(const std::string &databasePath)
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
            SatelliteData data;
            data.name = satellite["name"];
            data.number = satellite["satelliteNumber"];
            data.classification = satellite["classification"];
            data.launchYear = satellite["internationalDesignatorLaunchYear"];
            data.launchNumber = satellite["internationalDesignatorLaunchNumber"];
            data.launchPiece = satellite["internationalDesignatorPieceOfLaunch"];
            data.epochYear = satellite["epochYear"];
            data.epoch = satellite["epoch"];
            data.year = satellite["year"];
            data.month = satellite["month"];
            data.day = satellite["day"];
            data.hour = satellite["hour"];
            data.minute = satellite["minute"];
            data.inclination = satellite["inclination"];
            data.eccentricity = satellite["eccentricity"];
            data.argumentOfPerigee = satellite["argumentOfPerigee"];
            data.semiMajorAxis = satellite["semiMajorAxis"];
            data.longitudeOfAscendingNode = satellite["longitudeOfAscendingNode"];

            insert(data.name, data);
             
            // Push back the number into the selection map
            m_selectionMap.insert(std::make_pair(std::stoi(data.number), false));
        }
    }
    else
    {
        throw std::runtime_error("Database is not an array");
    }
}
