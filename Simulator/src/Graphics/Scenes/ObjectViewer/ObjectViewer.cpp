/**
 * @file ObjectViewer.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief
 * @version 0.1
 * @date 2023-07-14
 *
 *
 */

#include "ObjectViewer.h"

#include <imgui.h>

namespace Visualization
{
    ObjectViewer::ObjectViewer(uint32_t width, uint32_t height)
        : Scene(width, height)
    {
        m_Image = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
        m_imageBuffer.resize(width * height);
    }

    ObjectViewer::~ObjectViewer()
    {
    }

    void ObjectViewer::OnUpdate(float ts)
    {
        Clear();
        UpdateImage();
    }

    void ObjectViewer::OnUIRender()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("ObjectViewer");

        uint32_t windowWidth = (uint32_t)ImGui::GetWindowWidth();
        uint32_t windowHeight = (uint32_t)ImGui::GetWindowHeight();


        ImGui::Image(GetImage()->GetDescriptorSet(), ImVec2((float)GetWidth(), (float)GetHeight()));

        ResizeIfNeeded(windowWidth, windowHeight);

        ImGui::End();

        ImGui::PopStyleVar();
    }

    void ObjectViewer::Draw()
    {
    }

}