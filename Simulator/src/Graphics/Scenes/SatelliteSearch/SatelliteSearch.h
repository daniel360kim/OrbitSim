/**
 * @file SatelliteSearch.h
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
#include "SatelliteDatabase.h"

#include <glm/glm.hpp>

namespace Visualization
{
    class SatelliteSearch : public Scene
    {
    public:
        struct Commands
        {
            SceneSelection m_NextScene = SceneSelection::SatelliteSearch;
            std::vector<SatelliteData> m_SelectedSatellites;
        };

        SatelliteSearch(uint32_t width, uint32_t height);
        ~SatelliteSearch();

        void Draw();

        void OnUpdate(float ts);
        void OnUIRender(std::vector<ImFont *> &fonts);

        Commands &GetCommands() { return m_Commands; }

    private:
        Commands m_Commands;
        SatelliteDatabase m_Database;
        float m_LastRenderTime = 0.0f;

        char m_SearchQuery[256] = "";
        int m_NumSelected = 0;
        int m_NumResults = 50;

        void getTableData(const std::vector<SatelliteData>& data, std::vector<std::vector<std::string>>& rows);
    };

}