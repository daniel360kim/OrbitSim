/**
 * @file Application.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief application class
 * @version 0.1
 * @date 2023-06-24
 * 
 * 
 */

#include <iostream>
#include <GLFW/glfw3.h>

#include "Application.h"

Application::Application()
{
    m_Window = std::unique_ptr<Window>(Window::Create());
    m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
}

Application::~Application()
{
}

void Application::PushLayer(Layer* layer)
{
    m_LayerStack.PushLayer(layer);
}

void Application::PushOverlay(Layer* overlay)
{
    m_LayerStack.PushOverlay(overlay);
}

void Application::OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));

    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
    {
        (*--it)->OnEvent(e);
        if (e.Handled)
            break;
    }
}

bool Application::OnWindowClose(WindowCloseEvent& e)
{
    m_Running = false;
    return true;
}

void Application::run()
{
    while (m_Running)
    {
        glClearColor(1, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        for (Layer* layer : m_LayerStack)
            layer->OnUpdate();

        m_Window->OnUpdate();
    }
}