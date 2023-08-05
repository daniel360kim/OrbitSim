
#include "PlanetSearch.h"
#include "../Components/ImGui/Section.h"
#include "PlanetDatabase.h"

#include "util/Timer.h"

#include <iostream>
#include <iomanip>
#include <sstream>

namespace Visualization
{
    PlanetSearch::PlanetSearch(uint32_t width, uint32_t height)
        : Scene(width, height)
    {
        m_Database.load("assets/Data/exoplanets.json");
    }

    PlanetSearch::~PlanetSearch()
    {
    }

    void PlanetSearch::OnUpdate(float ts)
    {
        Clear();
        Draw();
        UpdateImage();
    }

    void PlanetSearch::OnUIRender(std::vector<ImFont *> &fonts)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("##PlanetSearch", nullptr, ImGuiWindowFlags_NoTitleBar);

        Section section(ImVec2(10, 10), fonts);

        section.Title("Planet Search");

        section.Separator();

        const std::vector<std::string> heading = {
            "Planet Name",
            "Planet ID",
            "Year Discovered",
            "Mass (kg)",
            "Radius (m)",
            "Planet/Exoplanet"
        };

        section.TextInput("Search", m_SearchQuery, sizeof(m_SearchQuery));

        std::vector<PlanetData> searchResults = m_Database.search(m_SearchQuery); // search the database

        section.Spacer(15.0f);
        section.Text("Found %d planets", searchResults.size());

        if (section.Button("Add Planet to Sim", ImVec2(150, 40), ImVec4(0.867f, 0.345f, 0.839f, 1.0f)))
        {
            m_Commands.m_NextScene = SceneSelection::OrbitViewer;
            m_Commands.m_SelectedPlanet = m_Database.m_selectedPlanet;
        }
        else
        {
            m_Commands.m_NextScene = SceneSelection::PlanetSearch;
        }

        section.Spacer(35.0f);

        // Convert the selection map to a vector of bools
        std::vector<bool> selectedRows(searchResults.size());

        if (m_Database.m_selectedPlanetIndex < searchResults.size())
            selectedRows[m_Database.m_selectedPlanetIndex] = true; // select the currently selected planet from the last call 

        std::vector<std::vector<std::string>> rows;
        getTableData(searchResults, rows);

        section.SingleSelectTable(heading.size(), heading, rows, "Select", "Deselect", selectedRows, 5000);

        // Update the selection map based on the table selections
        for (size_t i = 0; i < searchResults.size(); i++)
        {
            if (selectedRows[i])
            {
                m_Database.m_selectedPlanetIndex = i;
                m_Database.m_selectedPlanet = searchResults[i];
            }
        }

        section.End();

        ImGui::End();

        ImGui::PopStyleVar();
    }

    void PlanetSearch::Draw()
    {
        for (uint32_t i = 0; i < m_Width * m_Height; i++)
        {
            m_imageBuffer[i] = 0xFF000000;
        }
    }

    std::string PlanetSearch::doubleToScientificString(double value, int precision)
    {
        std::stringstream stream;
        stream << std::scientific << std::setprecision(precision) << value;
        return stream.str();
    }
    void PlanetSearch::getTableData(const std::vector<PlanetData> &data, std::vector<std::vector<std::string>> &rows)
    {
        rows.reserve(data.size());

        for (const auto &planet : data)
        {
            std::string planetMass = doubleToScientificString(planet.mass, 2);
            std::string planetRadius = doubleToScientificString(planet.radius, 2);

            std::string planetType;
            if (planet.type == PlanetType::GasGiant)
            {
                planetType = "Gas Giant";
            }
            else
            {
                planetType = "Terrestrial";
            }

            rows.emplace_back(std::vector<std::string>{planet.name, std::to_string(planet.id), std::to_string(planet.discoveryYear), planetMass, planetRadius, planetType});
        }
    }

}