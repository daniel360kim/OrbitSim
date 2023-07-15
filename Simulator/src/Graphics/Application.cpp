#include "Application.h"

#include <imgui.h>
#include <cstdint>

namespace Visualization
{
    Application::Application()
    {
        m_OpeningScene = std::make_shared<OpeningScene>(1280, 720);
        m_OrbitViewer = std::make_shared<OrbitViewer>(1280, 720);
        m_ObjectViewer = std::make_shared<ObjectViewer>(1280, 720);
        m_CurrentScene = Scene::OpeningScene;
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
                m_OpeningScene->OnUIRender();
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