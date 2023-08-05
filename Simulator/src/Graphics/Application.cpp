#include "Application.h"

#include <imgui.h>
#include <cstdint>

namespace Visualization
{
    Application::Application(std::vector<ImFont *> fonts)
    {
        m_SimulationTime.loadTimeFromFile();
        m_OpeningScene = std::make_shared<OpeningScene>(1280, 720);
        m_EarthOrbitViewer = std::make_shared<EarthOrbitViewer>(1280, 720);
        m_ObjectViewer = std::make_shared<ObjectViewer>(1280, 720);
        m_SatelliteSearch = std::make_shared<SatelliteSearch>(1280, 720);
        m_CurrentScene = SceneSelection::OpeningScene;
        m_Fonts = std::make_shared<std::vector<ImFont *>>(fonts);
    }

    void Application::OnUpdate(float ts)
    {
        m_SimulationTime.incrementTimeReal();

        switch (m_CurrentScene)
        {
        case SceneSelection::OpeningScene:
            m_OpeningScene->OnUpdate(ts, m_SimulationTime);
            break;
        case SceneSelection::EarthOrbitViewer:
            m_EarthOrbitViewer->OnUpdate(ts, m_SimulationTime);
            break;
        case SceneSelection::ObjectViewer:
            m_ObjectViewer->OnUpdate(ts, m_SimulationTime);
            break;
        case SceneSelection::SatelliteSearch:
            m_SatelliteSearch->OnUpdate(ts);
            break;
        default:
            throw std::runtime_error("Invalid scene selection");
        }

    }

    void Application::OnUIRender()
    {
        switch (m_CurrentScene)
        {
        case SceneSelection::OpeningScene:
        {
            m_OpeningScene->OnUIRender(*m_Fonts, m_SimulationTime);
            OpeningScene::Commands commands;
            m_CurrentScene = m_OpeningScene->GetCommands().m_NextScene;
            
            break;
        }
        case SceneSelection::EarthOrbitViewer:
        {
            m_EarthOrbitViewer->OnUIRender(*m_Fonts, m_SimulationTime);
            EarthOrbitViewer::Commands commands;
            m_CurrentScene = m_EarthOrbitViewer->GetCommands().m_NextScene;
            
            break;
        }
        case SceneSelection::ObjectViewer:
        {
            m_ObjectViewer->OnUIRender(*m_Fonts, m_SimulationTime);
            break;
        }
        case SceneSelection::SatelliteSearch:
        {
            m_SatelliteSearch->OnUIRender(*m_Fonts);
            SatelliteSearch::Commands commands;
            m_CurrentScene = m_SatelliteSearch->GetCommands().m_NextScene;

            if (m_CurrentScene == SceneSelection::EarthOrbitViewer)
            {
                m_EarthOrbitViewer->updateSatelliteData(m_SatelliteSearch->GetCommands().m_SelectedSatellites);
            }
            break;
        }

        default:
            throw std::runtime_error("Invalid scene selection");
        }
    }

}
