#include "Application.h"

#include <imgui.h>
#include <cstdint>

namespace Visualization
{
    Application::Application(std::vector<ImFont *> fonts)
    {
        m_SimulationTime.loadTimeFromFile();
        m_OpeningScene = std::make_shared<OpeningScene>(1280, 720);
        m_OrbitViewer = std::make_shared<OrbitViewer>(1280, 720);
        m_ObjectViewer = std::make_shared<ObjectViewer>(1280, 720);
        m_SatelliteSearch = std::make_shared<SatelliteSearch>(1280, 720);
        m_PlanetSearch = std::make_shared<PlanetSearch>(1280, 720);
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
        case SceneSelection::OrbitViewer:
            m_OrbitViewer->OnUpdate(ts, m_SimulationTime);
            break;
        case SceneSelection::ObjectViewer:
            m_ObjectViewer->OnUpdate(ts, m_SimulationTime);
            break;
        case SceneSelection::SatelliteSearch:
            m_SatelliteSearch->OnUpdate(ts);
            break;
        case SceneSelection::PlanetSearch:
            m_PlanetSearch->OnUpdate(ts);
            break;
        
        default:
            throw std::runtime_error("Invalid scene selection");
        }

    }

    void Application::OnUIRender()
    {
        std::cout << "Current Scene: " << static_cast<int>(m_CurrentScene) << std::endl;
        switch (m_CurrentScene)
        {
        case SceneSelection::OpeningScene:
        {
            m_OpeningScene->OnUIRender(*m_Fonts, m_SimulationTime);
            OpeningScene::Commands commands;
            m_CurrentScene = m_OpeningScene->GetCommands().m_NextScene;
            
            break;
        }
        case SceneSelection::OrbitViewer:
        {
            m_OrbitViewer->OnUIRender(*m_Fonts, m_SimulationTime);
            OrbitViewer::Commands commands;
            m_CurrentScene = m_OrbitViewer->GetCommands().m_NextScene;
            
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

            if (m_CurrentScene == SceneSelection::OrbitViewer)
            {
                m_OrbitViewer->updateSatelliteData(m_SatelliteSearch->GetCommands().m_SelectedSatellites);
            }
            break;
        }
        case SceneSelection::PlanetSearch:
        {
            m_PlanetSearch->OnUIRender(*m_Fonts);
            PlanetSearch::Commands commands;
            m_CurrentScene = m_PlanetSearch->GetCommands().m_NextScene;

            if (m_CurrentScene == SceneSelection::OrbitViewer)
            {
                m_OrbitViewer->updatePlanetData(m_PlanetSearch->GetCommands().m_SelectedPlanet);
            }

            break;
        }

        default:
            throw std::runtime_error("Invalid scene selection");
        }
    }

}
