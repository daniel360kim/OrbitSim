/**
 * @file EarthOrbitViewer.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief
 * @version 0.1
 * @date 2023-07-04
 *
 *
 */

#pragma once

#include <imgui.h>
#include <unordered_map>

#include "../Scene.h"
#include "../SceneSelection.h"
#include "../Components/Image.h"
#include "../Components/Bodies.h"
#include "../Components/Orbit.h"
#include "SimulationTime.h"
#include "../Camera.h"
#include "../SatelliteSearch/SatelliteData.h"

namespace Visualization
{
    class EarthOrbitViewer : public Scene
    {
    public:
        struct Commands
        {
            SceneSelection m_NextScene = SceneSelection::EarthOrbitViewer;
        };
        
        EarthOrbitViewer(uint32_t width, uint32_t height);
        ~EarthOrbitViewer();

        void setTargetOrbit(const std::string& name);

        void OnUpdate(float ts, SimulationTime &simulationTime);
        void OnUIRender(std::vector<ImFont *> &fonts, SimulationTime &simulationTime);

        void ResizeIfNeeded(uint32_t width, uint32_t height) override;

        void Draw(SimulationTime &simulationTime);
        void DrawBackground(std::shared_ptr<Image> background);

        void updateSatelliteData(const std::vector<SatelliteData>& data);

        Commands &GetCommands() { return m_Commands; }

    private:
        Commands m_Commands;

        std::shared_ptr<Image> m_SpaceBackground;
        std::shared_ptr<Body> m_Earth;
        std::shared_ptr<Camera> m_Camera;

        std::shared_ptr<Orbit> m_targetOrbit;
        std::unordered_map<std::string, std::shared_ptr<Orbit>> m_Orbits;
        size_t m_targetOrbitIndex = 0;
        std::vector<std::string> getBaseOrbitNames();

        const glm::vec4 m_baseOrbitColor = glm::vec4(1.0f, 0.906f, 0.608f, 1.0f);
        const glm::vec4 m_baseIconColor = glm::vec4(1.0f, 0.906f, 0.608f, 1.0f);
        const glm::vec4 m_targetOrbitColor = glm::vec4(0.867f, 0.354f, 0.839f, 1.0f);
        const glm::vec4 m_targetIconColor = glm::vec4(0.867f, 0.354f, 0.839f, 1.0f);

        float m_LastRenderTime = 0.0f;

        static std::vector<std::string> getBodyGUIItems(std::shared_ptr<Body> body);
        static std::vector<std::string> getOrbitGUIItems(std::shared_ptr<Ellipse> orbit);

    };
};
