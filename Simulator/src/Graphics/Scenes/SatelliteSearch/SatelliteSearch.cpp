
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


        const int resultsMap[5] = { 10, 20, 50, 100 }; // map for results per page dropdown
        std::string resultsText;
        resultsText += "Results per page: ";
        resultsText += std::to_string(m_NumResults);
        std::vector<bool> resultsFlags = section.DropDown("##Resultsss", resultsText.c_str(), { "10", "20", "50", "100" }); // results per page dropdown
        for (size_t i = 0; i < resultsFlags.size(); i++)
        {
            if (resultsFlags[i])
            {
                m_NumResults = resultsMap[i]; // use the results map to see which option was selected
            }
        }

        m_NumSelected = 0;

        if (section.Button("Add to Sim", ImVec2(150, 40), ImVec4(0.867f, 0.345f, 0.839f, 1.0f)))
        {
            m_Commands.m_NextScene = SceneSelection::OrbitViewer;
            m_Commands.m_SelectedSatellites.clear();

            // Search which satellite was selected using the ID and add it to the selected satellites vector
            for (auto& satellite : searchResults)
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

        std::vector<std::vector<std::string>> rows;
        getTableData(searchResults, rows);

        section.Table(heading.size(), heading, rows, "Select", "Deselect", selectedRows, m_NumResults);

        // Update the selection map based on the table selections
        for (size_t i = 0; i < searchResults.size(); i++)
        {
            if (selectedRows[i])
            {
                m_NumSelected++;
            }
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