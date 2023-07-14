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

#include "../orbit/OrbitalPropogator.h"

namespace Visualization
{
    ViewPort::ViewPort()
    {
        m_Renderer = std::make_shared<Visualization::Renderer>(960, 540);
        m_Earth = std::make_shared<Visualization::Body>(4.0f, 5, "../../Resources/earthDay.jpg", Visualization::Body::BodyType::Planet);
        m_Camera = std::make_shared<Visualization::Camera>();
        m_SpaceBackground = std::make_shared<Visualization::Image>("../../Resources/milkyway.jpg");

        CentralBody earth("earth", Type::Planet, 5.97219e24, 6371, 3);
        OrbitalObject moon = OrbitalObjectBuilder("Moon", Type::Planet, 7.34767309e22)
                                 .setSemiMajorAxis(7000.0)
                                 .setEccentricity(0.001)
                                 .setInclination(90.0)
                                 .setLongitudeOfAscendingNode(30.0)
                                 .setCentralBody(earth)
                                 .build();

        m_Orbit = std::make_shared<Visualization::Ellipse>(moon, 1);

        m_Orbit->propogateOrbit(m_Orbit->getOrbitalPeriod());

        m_Orbit->generateVertexPositions();
    
        m_Bodies.push_back(m_Earth);
    }

    ViewPort::~ViewPort()
    {
    }

    void ViewPort::OnUpdate(float ts)
    {
        m_Camera->OnUpdate(ts);

        m_Renderer->Clear();
        m_Renderer->Draw();
        m_Renderer->UpdateImage();
    }

    void ViewPort::OnUIRender()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("ViewPort");

        uint32_t windowWidth = (uint32_t)ImGui::GetWindowWidth();
        uint32_t windowHeight = (uint32_t)ImGui::GetWindowHeight();

        ImGui::Image(m_Renderer->GetImage()->GetDescriptorSet(), ImVec2((float)m_Renderer->GetWidth(), (float)m_Renderer->GetHeight()));

        m_Renderer->ResizeIfNeeded(windowWidth, windowHeight);

        ImGui::End();

        ImGui::PopStyleVar();
    }
}