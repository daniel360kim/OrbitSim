/**
 * @file Trie.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief 
 * @version 0.1
 * @date 2023-07-25
 * 
 * 
 */

#pragma once

#include "SatelliteData.h"
#include "util/Trie.h"

#include <vector>
#include <unordered_map>
#include <string>
#include <memory>

class SatelliteDatabase : public Trie<SatelliteData>
{
public:
    SatelliteDatabase();
    ~SatelliteDatabase();

    std::unordered_map<int, bool> m_selectionMap; // holds which IDs are selected within the search results

    void load(const std::string &databasePath);
};