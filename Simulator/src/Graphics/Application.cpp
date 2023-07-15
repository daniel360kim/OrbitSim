#include "Application.h"

#include <imgui.h>
#include <cstdint>

namespace Visualization
{
    Application::Application()
    {
        m_OrbitViewer = std::make_shared<Visualization::OrbitViewer>(1280, 720);
        m_ObjectViewer = std::make_shared<Visualization::ObjectViewer>(1280, 720);
        m_CurrentScene = Scene::ObjectViewer;
    }

    void Application::OnUpdate(float ts)
    {
        switch(m_CurrentScene)
        {
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
        switch(m_CurrentScene)
        {
            case Scene::OrbitViewer:
                m_OrbitViewer->OnUIRender();
                break;
            case Scene::ObjectViewer:
                m_ObjectViewer->OnUIRender();
                break;
        }
    }
}