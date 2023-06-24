/**
 * @file Application.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief application class
 * @version 0.1
 * @date 2023-06-24
 * 
 * 
 */

#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>

#include "Window.h"
#include "ApplicationEvent.h"

class Application
{
public:
    Application();
    ~Application();

    void OnEvent(Event& e);

    void run();
private:
    bool OnWindowClose(WindowCloseEvent& e);
    std::unique_ptr<Window> m_Window;
    bool m_Running = true;

};
#endif