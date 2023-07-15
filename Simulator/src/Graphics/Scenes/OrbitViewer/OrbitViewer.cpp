/**
 * @file ViewPort.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief
 * @version 0.1
 * @date 2023-07-04
 *
 *
 */

#include "OrbitViewer.h"

#include <imgui.h>
#include <iostream>
#include <unordered_map>
#include <glm/gtx/rotate_vector.hpp>
#include "OrbitViewer.h"

namespace Visualization
{

    OrbitViewer::OrbitViewer(uint32_t width, uint32_t height)
        : Scene(width, height)
    {
        m_Camera = std::make_shared<Camera>();

        m_SpaceBackground = std::make_shared<Visualization::Image>("../../Resources/Textures/milkyway.jpg");

        CentralBody earth("earth", Type::Planet, 5.97219e24, 6371, 3);

        m_Earth = std::make_shared<Visualization::CentralRenderBody>(earth, 5, "../../Resources/Textures/earthDay.jpg");

        OrbitalObject moon = OrbitalObjectBuilder("Moon", Type::Planet, 7.34767309e22)
                                 .setSemiMajorAxis(384400.0)
                                 .setEccentricity(0.0549)
                                 .setInclination(5.145)
                                 .setLongitudeOfAscendingNode(125.08)
                                 .setCentralBody(earth)
                                 .build();

        m_Orbit = std::make_shared<Visualization::Ellipse>(moon);
    }

    OrbitViewer::~OrbitViewer()
    {
    }

    void OrbitViewer::OnUpdate(float ts)
    {
        m_Camera->OnUpdate(ts);

        Clear();
        Draw();
        UpdateImage();
    }

    void OrbitViewer::OnUIRender()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("OrbitViewer");

        uint32_t windowWidth = (uint32_t)ImGui::GetWindowWidth();
        uint32_t windowHeight = (uint32_t)ImGui::GetWindowHeight();

        ImGui::Image(GetImage()->GetDescriptorSet(), ImVec2((float)GetWidth(), (float)GetHeight()));

        ResizeIfNeeded(windowWidth, windowHeight);

        ImGui::End();

        ImGui::PopStyleVar();
    }

    void OrbitViewer::ResizeIfNeeded(uint32_t width, uint32_t height)
    {
        if (m_Width == width && m_Height == height)
            return;

        m_Width = width;
        m_Height = height;

        m_Image = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);

        m_imageBuffer.resize(width * height);

        m_Earth->onResize(width, height);
    }

    void OrbitViewer::Draw()
    {
        DrawBackground(m_SpaceBackground);

        m_Earth->Draw(m_Camera->GetCameraInfo(), glm::vec2(m_Width / 2, m_Height / 2), m_imageBuffer);

        float offsetX = m_Width / 2;
        float offsetY = m_Height / 2;

        std::vector<uint32_t> ellipseImage(m_Width * m_Height, 0);

        for (size_t i = 0; i < m_Orbit->GetVertexPositions().size(); i++)
        {
            glm::vec3 position = m_Orbit->GetVertexPositions()[i];

            CameraInfo cameraInfo = m_Camera->GetCameraInfo();
            position *= cameraInfo.scale;
            position = glm::rotateX(position, m_Camera->GetPitch());
            position = glm::rotateY(position, m_Camera->GetYaw());

            position += cameraInfo.position;

            glm::vec2 pixelCoords = glm::vec2(position.x, position.y) * m_Earth->getScaleFactor() * 4.0f + glm::vec2(offsetX, offsetY);

            int x = static_cast<int>(pixelCoords.x);
            int y = static_cast<int>(pixelCoords.y);
            int index = y * m_Width + x;

            if (x >= 0 && x < m_Width && y >= 0 && y < m_Height)
            {
                m_imageBuffer[index] = 0xffffffff;
            }
        }
    }

    void OrbitViewer::DrawBackground(std::shared_ptr<Image> background)
    {

        // Draw the space background
        uint32_t *backgroundData = background->GetPixels();
        uint32_t backgroundWidth = background->GetWidth();
        uint32_t backgroundHeight = background->GetHeight();

        for (uint32_t y = 0; y < m_Height; y++)
        {
            for (uint32_t x = 0; x < m_Width; x++)
            {
                uint32_t backgroundX = (uint32_t)((float)x / m_Width * backgroundWidth);
                uint32_t backgroundY = (uint32_t)((float)y / m_Height * backgroundHeight);

                uint32_t backgroundIndex = backgroundY * backgroundWidth + backgroundX;

                m_imageBuffer[y * m_Width + x] = backgroundData[backgroundIndex];
            }
        }
    }
}