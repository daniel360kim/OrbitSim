
#include "SatelliteSearch.h"
#include "../Components/ImGui/Section.h"
#include "SatelliteDatabase.h"

#include "util/Timer.h"

#include <iostream>

namespace Visualization
{
    SatelliteSearch::SatelliteSearch(uint32_t width, uint32_t height)
        : Scene(width, height)
    {
        m_Database.load("assets/Data/satellites.json");
    }

    SatelliteSearch::~SatelliteSearch()
    {
    }

    void SatelliteSearch::OnUpdate(float ts)
    {
        Clear();
        Draw();
        UpdateImage(); 
    }

    void SatelliteSearch::OnUIRender(std::vector<ImFont *> &fonts)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("SatSearch", nullptr, ImGuiWindowFlags_NoTitleBar);

        Section section(ImVec2(10, 10), fonts);

        section.Title("Satellite Search");

        section.Separator();

        const std::vector<std::string> heading = {
            "Name",
            "ID"
        };

        section.TextInput("Search", m_SearchQuery, sizeof(m_SearchQuery));

        std::vector<SatelliteData> searchResults = m_Database.search(m_SearchQuery); // search the database

        section.Spacer(15.0f);
        section.Text("Found %d results", searchResults.size());
        section.Text("Selected: %d", m_NumSelected);

        if (section.Button("Add to Sim", ImVec2(150, 40), ImVec4(0.867f, 0.345f, 0.839f, 1.0f)))
        {
            m_Commands.m_NextScene = SceneSelection::EarthOrbitViewer;
            m_Commands.m_SelectedSatellites.clear();

            // Search which satellite was selected using the ID and add it to the selected satellites vector
            for (auto& satellite : m_Database.m_allSatellites)
            {
                if (m_Database.m_selectionMap[std::stoi(satellite.number)])
                {
                    m_Commands.m_SelectedSatellites.push_back(satellite);
                }
            }

        }
        else 
        {
            m_Commands.m_NextScene = SceneSelection::SatelliteSearch;
        }

        bool deselectAll = section.InlineButton("Deselect All", ImVec2(150, 40));
        bool selectAll = section.InlineButton("Select All", ImVec2(150, 40));
        section.Spacer(35.0f);

        // Convert the selection map to a vector of bools
        std::vector<bool> selectedRows(searchResults.size());
        if (deselectAll)
        {
            for (size_t i = 0; i < searchResults.size(); i++)
            {
                selectedRows[i] = false;
            }
        }
        else if (selectAll)
        {
            for (size_t i = 0; i < searchResults.size(); i++)
            {
                selectedRows[i] = true;
            }
        }
        else
        {
            for (size_t i = 0; i < searchResults.size(); i++)
            {
                selectedRows[i] = m_Database.m_selectionMap[std::stoi(searchResults[i].number)];
            }
        }

        int trueCount = 0;
        for (bool value : selectedRows)
        {
            if (value)
            {
                trueCount++;
            }
        }

        std::vector<std::vector<std::string>> rows;
        getTableData(searchResults, rows);

        section.Table(heading.size(), heading, rows, "Select", "Deselect", selectedRows, 10000);

        trueCount = 0;
        for (bool value : selectedRows)
        {
            if (value)
            {
                trueCount++;
            }
        }

        // Update the selection map based on the table selections'
        m_NumSelected = m_Database.m_selectionMap.size();
        for (size_t i = 0; i < m_Database.m_selectionMap.size(); i++)
        {
            if (!m_Database.m_selectionMap[i])
            {
                m_NumSelected--;
            }
        }
        for (size_t i = 0; i < searchResults.size(); i++)
        {
            m_Database.m_selectionMap[std::stoi(searchResults[i].number)] = selectedRows[i];
        }

        

        section.End();
       
        ImGui::End();

        ImGui::PopStyleVar();
        
    }


    void SatelliteSearch::Draw()
    {
        for (uint32_t i = 0; i < m_Width * m_Height; i++)
        {
            m_imageBuffer[i] = 0xFF000000;
        }
    }

    void SatelliteSearch::getTableData(const std::vector<SatelliteData>& data, std::vector<std::vector<std::string>>& rows)
    {
        rows.reserve(data.size());

        for (const auto& satellite : data)
        {
            rows.emplace_back(std::vector<std::string>{ satellite.name, satellite.number });
        }
    }

}