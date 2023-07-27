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
    m_Root = std::make_unique<TrieNode>();

}
SatelliteDatabase::~SatelliteDatabase()
{

}

std::vector<SatelliteData> SatelliteDatabase::search(const std::string &key)
{
    std::vector<SatelliteData> results;
    TrieNode *current = m_Root.get();

    for (size_t i = 0; i < key.length(); i++)
    {
        char character = key[i];
        character = std::tolower(character);
        if (current->children.find(character) == current->children.end())
        {
            return results;
        }
        current = current->children[character].get();
    }

    if (current->children.find('\0') != current->children.end())
    {
        results = current->children['\0']->data;
    }

    return results;
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

void SatelliteDatabase::insert(const std::string &key, const SatelliteData &data)
{
    TrieNode *current = m_Root.get();

    for (size_t i = 0; i < key.length(); i++)
    {
        char character = key[i];
        character = std::tolower(character);
        if (current->children.find(character) == current->children.end())
        {
            current->children[character] = std::make_unique<TrieNode>();
        }
        current = current->children[character].get();

        std::string substring = key.substr(0, i + 1);
        if (current->children.find('\0') == current->children.end())
        {
            current->children['\0'] = std::make_unique<TrieNode>();
        }
        current->children['\0']->data.push_back(data);
    }
    current->isEndOfWord = true;
    current->data.push_back(data);
}