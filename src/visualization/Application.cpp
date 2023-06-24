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
#include "Application.h"

Application::Application()
{
    m_Window = std::unique_ptr<Window>(Window::Create());
    m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
}

Application::~Application()
{
}

void Application::OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));

    
    //std::cout << e << std::endl; for debugging events
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
        m_Window->OnUpdate();
    }
}