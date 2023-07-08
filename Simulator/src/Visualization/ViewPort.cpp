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
        m_Sphere = std::make_shared<Visualization::Sphere>(1, 6, "../../Resources/earth.jpg");
        m_Camera = std::make_shared<Visualization::Camera>();
    }

    ViewPort::~ViewPort()
    {
    }

    void ViewPort::OnUpdate(float ts)
    {

        m_Camera->OnUpdate(ts);
        m_Renderer->Clear(0x0000FFFF);

        Timer timer("Draw");
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