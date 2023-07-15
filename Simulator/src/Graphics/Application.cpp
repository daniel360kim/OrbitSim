#include "Application.h"

#include <imgui.h>
#include <cstdint>

namespace Visualization
{
    Application::Application(std::vector<ImFont*> fonts)
    {
        m_OpeningScene = std::make_shared<OpeningScene>(1280, 720);
        m_OrbitViewer = std::make_shared<OrbitViewer>(1280, 720);
        m_ObjectViewer = std::make_shared<ObjectViewer>(1280, 720);
        m_CurrentScene = Scene::OpeningScene;
        m_Fonts = std::make_shared<std::vector<ImFont*>>(fonts);
    }

    void Application::OnUpdate(float ts)
    {
        switch(m_CurrentScene)
        {
            case Scene::OpeningScene:
                m_OpeningScene->OnUpdate(ts);
                break;
            case Scene::OrbitViewer:
                m_OrbitViewer->OnUpdate(ts);
                break;
            case Scene::ObjectViewer:
                m_ObjectViewer->OnUpdate(ts);
                break;
        }

    }

    void Application::OnUIRender()
    {
        Commands commands;
        switch(m_CurrentScene)
        {
            case Scene::OpeningScene:
                m_OpeningScene->OnUIRender(*m_Fonts);
                commands = m_OpeningScene->GetCommands();
                if(commands.m_Exit)
                {
                    m_CurrentScene = Scene::OrbitViewer;
                }
                break;
            case Scene::OrbitViewer:
                m_OrbitViewer->OnUIRender();
                break;
            case Scene::ObjectViewer:
                m_ObjectViewer->OnUIRender();
                break;
        }
    }
}