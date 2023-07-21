/**
 * @file ViewPort.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief
 * @version 0.1
 * @date 2023-07-04
 *
 *
 */
#define _USE_MATH_DEFINES
#include <cmath>

#include "OrbitViewer.h"
#include "../Components/ImGui/Section.h"

#include <iostream>
#include <unordered_map>
#include <iomanip>
#include <sstream>
#include <glm/gtx/rotate_vector.hpp>
#include <Walnut/Timer.h>

namespace Visualization
{
    OrbitViewer::OrbitViewer(uint32_t width, uint32_t height)
        : Scene(width, height)
    {
        m_Camera = std::make_shared<Camera>();

        // Load the space background
        m_SpaceBackground = std::make_shared<Visualization::Image>("../../assets/Textures/milkyway.jpg");

        // Load the earth, set as the central body
        CentralBody earth("Earth", Type::Planet, 5.97219e24, 6371, 3);
        m_Earth = std::make_shared<Body>(earth, 5, width, height, "../../assets/Textures/earthDay.jpg");

        // Load orbits
        OrbitalObject moon = OrbitalObjectBuilder("Moon", Type::Moon, 7.34767309e22)
                                 .setSemiMajorAxis(384400.0)
                                 .setEccentricity(0.0549)
                                 .setInclination(5.145)
                                 .setLongitudeOfAscendingNode(125.08)
                                 .setCentralBody(earth)
                                 .build();
        
        OrbitalObject satelliteA = OrbitalObjectBuilder("SatelliteA", Type::Satellite, 34)
                                .setSemiMajorAxis(150000.0)
                                .setEccentricity(0.0)
                                .setInclination(0.0)
                                .setLongitudeOfAscendingNode(0.0)
                                .setCentralBody(earth)
                                .build();

        OrbitalObject satelliteB = OrbitalObjectBuilder("SatelliteB", Type::Satellite, 150)
                                .setSemiMajorAxis(200000.0)
                                .setEccentricity(0.3)
                                .setInclination(45.0)
                                .setCentralBody(earth)
                                .build();


        m_Orbits.insert(std::make_pair(moon.getName(), std::make_shared<Orbit>(moon, m_baseOrbitColor, m_baseIconColor, width, height)));
        m_Orbits.insert(std::make_pair(satelliteA.getName(), std::make_shared<Orbit>(satelliteA, m_baseOrbitColor, m_baseIconColor, width, height)));
        m_Orbits.insert(std::make_pair(satelliteB.getName(), std::make_shared<Orbit>(satelliteB, m_baseOrbitColor, m_baseIconColor, width, height)));

        setTargetOrbit("Moon");

    }

    OrbitViewer::~OrbitViewer()
    {
    }

    void OrbitViewer::setTargetOrbit(const std::string& name)
    {
        m_targetOrbit = m_Orbits[name];

        for (const auto& pair : m_Orbits)
        {
            const std::string& key = pair.first;
            const std::shared_ptr<Orbit>& orbit = pair.second;
            orbit->setOrbitColor(m_baseOrbitColor);
            orbit->setIconColor(m_baseIconColor);
        }

        m_targetOrbit->setOrbitColor(m_targetOrbitColor);
        m_targetOrbit->setIconColor(m_targetOrbitColor);
    }

    void OrbitViewer::OnUpdate(float ts, SimulationTime &simulationTime)
    {
        Walnut::Timer timer;
        m_Camera->OnUpdate(ts);

        Clear();
        Draw(simulationTime);
        UpdateImage();

        m_LastRenderTime = timer.ElapsedMillis();
    }

    void OrbitViewer::OnUIRender(std::vector<ImFont*> &fonts, SimulationTime &simulationTime)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        // Set this window to take up 3/4 of the screen
        ImGui::Begin("OrbitViewer", nullptr, ImGuiWindowFlags_NoScrollbar);

        uint32_t windowWidth = (uint32_t)ImGui::GetWindowWidth();
        uint32_t windowHeight = (uint32_t)ImGui::GetWindowHeight();

        ImGui::Image(GetImage()->GetDescriptorSet(), ImVec2((float)GetWidth(), (float)GetHeight()));

        ResizeIfNeeded(windowWidth, windowHeight);

        ImGui::End();

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
        ImGui::Begin("Properties");

        windowWidth = (uint32_t)ImGui::GetWindowWidth();
        windowHeight = (uint32_t)ImGui::GetWindowHeight();

        Section propertiesSection(ImVec2(10, 10), fonts);

        propertiesSection.Title("Properties");
        propertiesSection.Separator();

        // Time controls
        propertiesSection.Heading("Time Controls");
        propertiesSection.Text("Current Date: %s", simulationTime.getFormattedDate().c_str());
        propertiesSection.Text("Current Time: %s", simulationTime.getFormattedTime().c_str());

        if (propertiesSection.Button("Accelerate Time"))
        {
            simulationTime.accelerateTime(2);
        }

        if (propertiesSection.InlineButton("Decelerate Time"))
        {
            simulationTime.decelerateTime(2);
        }

        if (propertiesSection.InlineButton("Reset Acceleration"))
        {
            float currentAcceleration = simulationTime.getTimeScale();
            if (currentAcceleration == 0)
            {
                simulationTime.setTimeScale(1);
            }
            else if (currentAcceleration > 1)
            {
                simulationTime.decelerateTime(currentAcceleration);
            }
            else if (currentAcceleration < 1)
            {
                simulationTime.accelerateTime(1.0 / currentAcceleration);
            }
        }

        propertiesSection.Spacer();
        propertiesSection.Text("Time Multiplier: x%.2f", simulationTime.getTimeScale());

        if (propertiesSection.Button("Pause Time"))
        {
            simulationTime.setTimeScale(0);
        }

        if (propertiesSection.InlineButton("Resume Time"))
        {
            simulationTime.setTimeScale(1);
        }

        if (propertiesSection.InlineButton("Reset Time"))
        {
            simulationTime.resetTime();
        }

        propertiesSection.Separator();

        propertiesSection.Heading("Camera");

        // Camera kinematics
        CameraInfo cameraInfo = m_Camera->GetCameraInfo();
        propertiesSection.Text("Position: %.2f, %.2f, %.2f", cameraInfo.position.x, cameraInfo.position.y, cameraInfo.position.z);
        propertiesSection.Text("Scale: %.2f %", cameraInfo.scale * 100.0f);
        constexpr float RAD_TO_DEG = 180.0f / M_PI;
        propertiesSection.Text("Pitch: %.2f Yaw: %.2f", cameraInfo.pitch * RAD_TO_DEG, cameraInfo.yaw * RAD_TO_DEG);

        // Camera controls

        propertiesSection.SliderFloat("Zoom", &m_Camera->m_zoomSpeed, 1.0f, 10.0f, "%.0f");
        propertiesSection.SliderFloat("Rotation", &m_Camera->m_rotationSpeed, 1.0f, 10.0f, "%.0f");
        propertiesSection.SliderFloat("Pan", &m_Camera->m_movementSpeed, 1.0f, 10.0f, "%.0f");

        propertiesSection.Separator();

        // Central body properties
        propertiesSection.Heading(m_Earth->GetCentralBody().getName().c_str());
        std::vector<std::string> centralRenderBodyInfo = getBodyGUIItems(m_Earth);
        for (auto &item : centralRenderBodyInfo)
            propertiesSection.Text(item.c_str());

        // Central body resdolution controls
        int newResolution = m_Earth->GetSubdivisionLevel();
        propertiesSection.SliderInt("Resolution", &newResolution, 2, 7, "%d");
        m_Earth->changeSubdivisionLevel(newResolution);

        propertiesSection.Separator();

        // Target orbit
        std::string targetOrbitText = "Target Orbit: " + m_targetOrbit->GetOrbitalObject().getName();
        propertiesSection.Heading(targetOrbitText.c_str());


        std::vector<bool> results = propertiesSection.DropDown("##Target", "Select a new target orbit", getBaseOrbitNames());

        for (size_t i = 0; i < results.size(); i++)
        {
            if (results[i])
            {
                setTargetOrbit(getBaseOrbitNames()[i]);
            }
        }

        std::vector<std::string> targetInfo = getOrbitGUIItems(m_targetOrbit);
        for (auto &item : targetInfo)
            propertiesSection.Text(item.c_str());

        if (propertiesSection.Button("View Object"))
        {
            m_Commands.m_ViewObject = true;
           // m_Commands.m_Object = m_targetOrbit->GetOrbitalObject().getCentralBody();
        }

        propertiesSection.Separator();

        // Performance
        propertiesSection.Heading("Performance");
        propertiesSection.Text("Last Render Time: %.3f ms", m_LastRenderTime);

        propertiesSection.Separator();

        propertiesSection.End();

        ImGui::End();

        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
    }

    void OrbitViewer::ResizeIfNeeded(uint32_t width, uint32_t height)
    {
        if (m_Width == width && m_Height == height)
        {
            return;
        }
        m_Width = width;
        m_Height = height;

        m_Image = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);

        m_imageBuffer.resize(width * height);

        m_Earth->onResize(width, height);
        
        for (const auto& pair : m_Orbits)
        {
            const std::string& key = pair.first;
            const std::shared_ptr<Orbit>& orbit = pair.second;
            orbit->onResize(width, height);
        }
    }

    void OrbitViewer::Draw(SimulationTime &simulationTime)
    {
        DrawBackground(m_SpaceBackground);

        m_Earth->Draw(m_Camera->GetCameraInfo(), glm::vec2(m_Width / 2, m_Height / 2), m_imageBuffer);

        for (const auto& pair : m_Orbits)
        {
            const std::string& key = pair.first;
            const std::shared_ptr<Orbit>& orbit = pair.second;
            orbit->DrawOrbit(m_Camera->GetCameraInfo(), glm::vec2(m_Width / 2, m_Height / 2), m_imageBuffer, simulationTime.getTime());
            orbit->DrawIcon(m_Camera->GetCameraInfo(), m_imageBuffer);
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

    /**
     * @brief Gets the names of all the orbits, except the target orbit, Used in the dropdown
     * 
     * @return std::vector<std::string> 
     */
    std::vector<std::string> OrbitViewer::getBaseOrbitNames()
    {
        std::vector<std::string> names;
        for (const auto& pair : m_Orbits)
        {
            const std::string& key = pair.first;
            if (key != m_targetOrbit->GetOrbitalObject().getName())
                names.push_back(key);
        }
        return names;
    }

    std::vector<std::string> OrbitViewer::getOrbitGUIItems(std::shared_ptr<Ellipse> orbit)
    {
        std::vector<std::string> items;

        std::string line;

        line = "Type: " + orbit->GetOrbitalObject().getTypeString();
        items.push_back(line);

        line = "Mass: " + orbit->GetOrbitalObject().getMassScientific(2) + " kg";
        items.push_back(line);

        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << orbit->GetOrbitalObject().getSemiMajorAxis();
        line = "Semi-Major Axis: " + ss.str() + " km";
        items.push_back(line);

        ss.str("");
        ss << std::fixed << std::setprecision(2) << orbit->GetOrbitalObject().getEccentricity();
        line = "Eccentricity: " + ss.str();
        items.push_back(line);

        ss.str("");
        ss << std::fixed << std::setprecision(2) << orbit->GetOrbitalObject().getInclination();
        line = "Inclination: " + ss.str() + " deg";
        items.push_back(line);

        ss.str("");
        ss << std::fixed << std::setprecision(2) << orbit->GetOrbitalObject().calculateApogee();
        line = "Apoapsis: " + ss.str() + " km";
        items.push_back(line);

        ss.str("");
        ss << std::fixed << std::setprecision(2) << orbit->GetOrbitalObject().calculatePerigee();
        line = "Periapsis: " + ss.str() + " km";
        items.push_back(line);

        return items;
    }

    std::vector<std::string> OrbitViewer::getBodyGUIItems(std::shared_ptr<Body> body)
    {
        std::vector<std::string> items;

        std::string line;

        line = "Type: " + body->GetCentralBody().getTypeString();
        items.push_back(line);

        line = "Mass: " + body->GetCentralBody().getMassScientific(2) + " kg";
        items.push_back(line);

        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << body->GetCentralBody().getRadius();
        line = "Radius: " + ss.str() + " km";
        items.push_back(line);

        return items;
    }

}
