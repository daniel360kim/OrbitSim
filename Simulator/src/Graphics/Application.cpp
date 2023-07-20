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
        m_CurrentScene = Scene::OpeningScene;
        m_Fonts = std::make_shared<std::vector<ImFont *>>(fonts);

        SimulationTime::Time time;
        time.years = 2021;
        time.months = 1;
        time.days = 12;
        time.hours = 6;
        time.minutes = 0;
        time.seconds = 24;
        time.milliseconds = 0;
    }

    void Application::OnUpdate(float ts)
    {
        m_SimulationTime.incrementTimeReal();

        switch (m_CurrentScene)
        {
        case Scene::OpeningScene:
            m_OpeningScene->OnUpdate(ts, m_SimulationTime);
            break;
        case Scene::OrbitViewer:
            m_OrbitViewer->OnUpdate(ts, m_SimulationTime);
            break;
        case Scene::ObjectViewer:
            m_ObjectViewer->OnUpdate(ts, m_SimulationTime);
            break;
        }
    }

    void Application::OnUIRender()
    {
        switch (m_CurrentScene)
        {
        case Scene::OpeningScene:
        {
            m_OpeningScene->OnUIRender(*m_Fonts, m_SimulationTime);
            OpeningScene::Commands commands;
            commands = m_OpeningScene->GetCommands();
            if (commands.m_Exit)
            {
                m_CurrentScene = Scene::OrbitViewer;
            }
            break;
        }
        case Scene::OrbitViewer:
        {
            m_OrbitViewer->OnUIRender(*m_Fonts, m_SimulationTime);
            OrbitViewer::Commands commands;
            commands = m_OrbitViewer->GetCommands();
            if (commands.m_ViewObject)
            {
                m_CurrentScene = Scene::ObjectViewer;
            }
            break;
        }
        case Scene::ObjectViewer:
        {
            m_ObjectViewer->OnUIRender(*m_Fonts, m_SimulationTime);
            break;
        }
        }
    }

}
