#include "Application.h"

#include <imgui.h>
#include <cstdint>

namespace Visualization
{
    Application::Application()
    {
        m_OrbitViewer = std::make_shared<Visualization::OrbitViewer>(1280, 720);
    }
    void Application::OnUpdate(float ts)
    {
        m_OrbitViewer->update(ts);
    }
    void Application::OnUIRender()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("OrbitViewer");

        uint32_t windowWidth = (uint32_t)ImGui::GetWindowWidth();
        uint32_t windowHeight = (uint32_t)ImGui::GetWindowHeight();

        ImGui::Image(m_OrbitViewer->GetImage()->GetDescriptorSet(), ImVec2((float)m_OrbitViewer->GetWidth(), (float)m_OrbitViewer->GetHeight()));

        m_OrbitViewer->ResizeIfNeeded(windowWidth, windowHeight);

        ImGui::End();

        ImGui::PopStyleVar();
    }
}