/**
 * @file PlanetSearch.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief 
 * @version 0.1
 * @date 2023-07-24
 * 
 * 
 */
#pragma once

#include "../Scene.h"
#include "../SceneSelection.h"
#include "../Components/Image.h"
#include "PlanetDatabase.h"
#include "orbit/CentralBody.h"

#include <glm/glm.hpp>

namespace Visualization
{
    class PlanetSearch : public Scene
    {
    public:
        struct Commands
        {
            SceneSelection m_NextScene = SceneSelection::PlanetSearch;
            PlanetData m_SelectedPlanet;
        };

        PlanetSearch(uint32_t width, uint32_t height);
        ~PlanetSearch();

        void Draw();

        void OnUpdate(float ts);
        void OnUIRender(std::vector<ImFont *> &fonts);

        Commands &GetCommands() { return m_Commands; }

    private:
        Commands m_Commands;
        PlanetDatabase m_Database;
        float m_LastRenderTime = 0.0f;

        char m_SearchQuery[256] = "";

        static inline std::string doubleToScientificString(double value, int precision);
        void getTableData(const std::vector<PlanetData>& data, std::vector<std::vector<std::string>>& rows);
    };

}