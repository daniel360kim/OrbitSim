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

#include <vector>
#include <unordered_map>
#include <string>
#include <memory>

struct TrieNode
{
    std::unordered_map<char, std::unique_ptr<TrieNode>> children;
    std::vector<SatelliteData> data;
    bool isEndOfWord;
};

class SatelliteDatabase
{
public:
    SatelliteDatabase();
    ~SatelliteDatabase();

    std::vector<SatelliteData> search(const std::string &key);
    std::unordered_map<int, bool> m_selectionMap; // holds which IDs are selected within the search results
    std::vector<SatelliteData> m_allSatellites; // holds all satellites in the database

    void load(const std::string &databasePath);

private:
    void insert(const std::string &key, const SatelliteData &data);

    std::unique_ptr<TrieNode> m_Root;
};