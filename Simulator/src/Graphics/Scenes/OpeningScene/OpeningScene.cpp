/**
 * @file OpeningScene.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief
 * @version 0.1
 * @date 2023-07-14
 *
 *
 */

#include "OpeningScene.h"
#include "orbit/CentralBody.h"
#include "../Components/ImageResizer.h"

#include <imgui.h>
#include <glm/gtx/rotate_vector.hpp>
#include <Walnut/Timer.h>

namespace Visualization
{
    OpeningScene::OpeningScene(uint32_t width, uint32_t height)
        : Scene(width, height)
    {

        m_SpaceBackground = std::make_shared<Image>("assets/Images/milkyway.jpg");
        m_Camera = std::make_shared<OpeningSceneCamera>();

        CentralBody mars("mars", Type::Planet, 6.4171e23, 3389.5, 3);

        m_Mars = std::make_shared<Visualization::Body>(mars, 6, width, height, "assets/Textures/mars.jpg");
    }

    OpeningScene::~OpeningScene()
    {
    }

    void OpeningScene::OnUpdate(float ts, SimulationTime &simulationTime)
    {
        Walnut::Timer timer;
        m_Camera->OnUpdate(ts);
        Clear();
        Draw();
        UpdateImage(); 

        m_LastRenderTime = timer.ElapsedMillis();
    }

    void OpeningScene::OnUIRender(std::vector<ImFont *> &fonts, SimulationTime &simulationTime)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
        ImGui::SetNextWindowSize(ImVec2(m_Width, m_Height));

        ImGui::Begin("OrbitSim", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysAutoResize);
        ImGuiStyle &style = ImGui::GetStyle();

        uint32_t windowWidth = (uint32_t)ImGui::GetWindowWidth();
        uint32_t windowHeight = (uint32_t)ImGui::GetWindowHeight();

        ImGui::Image(GetImage()->GetDescriptorSet(), ImVec2((float)GetWidth(), (float)GetHeight()));

        float titleX = windowWidth / 18;
        float titleY = windowHeight / 8;
        ImGui::SetCursorPos(ImVec2(titleX, titleY));
        style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 0.906f, 0.608f, 1.0f);
        ImGui::PushFont(fonts[1]);
        ImGui::Text("OrbitSim");

        ImGui::PopFont();

        // Set cursor right below the title
        ImGui::SetCursorPos(ImVec2(titleX, titleY + 300));

        // Save the current button color and hover color
        ImVec4 defaultButtonColor = ImGui::GetStyleColorVec4(ImGuiCol_Button);
        ImVec4 defaultHoverColor = ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered);
        ImVec4 defaultBackgroundColor = ImGui::GetStyleColorVec4(ImGuiCol_WindowBg);

        // Set custom button and hover colors
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.906f, 0.608f, 0.8f));

        // Set custom button border properties
        style.FrameBorderSize = 1.0f; // Set border thickness
        style.Colors[ImGuiCol_Border] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

        // Set custom text color
        style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

        if (ImGui::Button("Start", ImVec2(150, 75)))
        {
            m_Commands.m_NextScene = SceneSelection::EarthOrbitViewer;
        }
        // Restore the default button and hover colors
        ImGui::PopStyleColor(2);

        // Restore the default button border properties
        style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        style.FrameBorderSize = 0.0f;
        style.Colors[ImGuiCol_Border] = ImVec4(0, 0, 0, 0);

        ResizeIfNeeded(windowWidth, windowHeight);

        ImGui::End();

        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
    }

    void OpeningScene::ResizeIfNeeded(uint32_t width, uint32_t height)
    {
        if (m_Width == width && m_Height == height)
            return;

        m_Width = width;
        m_Height = height;

        m_Image = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);

        m_imageBuffer.resize(width * height);

        m_Mars->onResize(width, height);
    }

    void OpeningScene::Draw()
    {
        DrawBackground();

        // Set offset so the body is in the first third of the screen (from the left) and in the middle of the screen (from the top)
        glm::vec2 offset = glm::vec2(m_Width / 5, m_Height + (m_Height / 6));
        m_Mars->Draw(m_Camera->GetCameraInfo(), offset, m_imageBuffer);
    }

    void OpeningScene::DrawBackground()
    {
        // Draw the space background
        uint32_t *backgroundData = m_SpaceBackground->GetPixels();
        uint32_t backgroundWidth = m_SpaceBackground->GetWidth();
        uint32_t backgroundHeight = m_SpaceBackground->GetHeight();

        // Add to image buffer. Fit only the center of the image if the image is bigger than the screen
        for (uint32_t y = 0; y < m_Height; y++)
        {
            for (uint32_t x = 0; x < m_Width; x++)
            {
                m_imageBuffer[y * m_Width + x] = backgroundData[(y % backgroundHeight) * backgroundWidth + (x % backgroundWidth)];
            }
        }
    }
}