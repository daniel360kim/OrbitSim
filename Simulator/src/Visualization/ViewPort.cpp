/**
 * @file ViewPort.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief
 * @version 0.1
 * @date 2023-07-04
 *
 *
 */

#include "ViewPort.h"

#include <imgui.h>

#include "../util/Timer.h"

namespace Visualization
{
    ViewPort::ViewPort()
    {
        m_Renderer = std::make_shared<Visualization::Renderer>(960, 540);
        m_Earth = std::make_shared<Visualization::Body>(40, 5, "../../Resources/earthDay.jpg", "Earth", Visualization::Body::BodyType::Planet);
        m_Sun = std::make_shared<Visualization::Body>(20, 5, "../../Resources/sun.jpg", "Sun", Visualization::Body::BodyType::Star);
        m_Moon = std::make_shared<Visualization::Body>(10, 5, "../../Resources/moon.jpg", "Moon", Visualization::Body::BodyType::Moon);
        m_Camera = std::make_shared<Visualization::Camera>();
        m_SpaceBackground = std::make_shared<Visualization::Image>("../../Resources/milkyway.jpg");



        m_Bodies.push_back(m_Earth);
        m_Bodies.push_back(m_Moon);
        //m_Bodies.push_back(m_Sun);
    }

    ViewPort::~ViewPort()
    {
    }

    void ViewPort::OnUpdate(float ts)
    {
        m_Camera->OnUpdate(ts);
        m_Renderer->Clear(0xFF000000);

        Timer timer("Renderer::Draw");
        m_Renderer->Draw();
        timer.Stop();
        timer.printResults();
        
        m_Renderer->UpdateImage();
    }
    void ViewPort::OnUIRender()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Sim");

        uint32_t windowWidth = (uint32_t)ImGui::GetWindowWidth();
        uint32_t windowHeight = (uint32_t)ImGui::GetWindowHeight();

        ImGui::Image(m_Renderer->GetImage()->GetDescriptorSet(), ImVec2((float)m_Renderer->GetWidth(), (float)m_Renderer->GetHeight()));

        m_Renderer->ResizeIfNeeded(windowWidth, windowHeight);

        ImGui::End();

        ImGui::PopStyleVar();
    }
}