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

namespace Visualization
{
    ViewPort::ViewPort()
        : m_Camera(960, 540)
    {
        m_Renderer = std::make_shared<Visualization::Renderer>(960, 540);
        m_Sphere = std::make_shared<Visualization::Sphere>(1, 100, 100);

    }

    ViewPort::~ViewPort()
    {
    }

    void ViewPort::OnUpdate(float ts)
    {
        m_Camera.OnUpdate(ts);

        m_Renderer->Clear(0x0000FFFF);
        m_Renderer->Draw(0, 0, m_Camera);
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