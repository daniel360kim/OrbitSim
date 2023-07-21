/**
 * @file OrbitViewer.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief
 * @version 0.1
 * @date 2023-07-04
 *
 *
 */

#pragma once

#include <imgui.h>

#include "../Scene.h"
#include "../Components/Image.h"
#include "../Components/Bodies.h"
#include "../Components/Orbit.h"
#include "SimulationTime.h"
#include "../Camera.h"

namespace Visualization
{
    class OrbitViewer : public Scene
    {
    public:
        struct Commands
        {
            bool m_ViewObject = false;
            std::shared_ptr<Body> m_Object;
        };
        OrbitViewer(uint32_t width, uint32_t height);
        ~OrbitViewer();

        void OnUpdate(float ts, SimulationTime &simulationTime);
        void OnUIRender(std::vector<ImFont *> &fonts, SimulationTime &simulationTime);

        void ResizeIfNeeded(uint32_t width, uint32_t height) override;

        void Draw(SimulationTime &simulationTime);
        void DrawBackground(std::shared_ptr<Image> background);

        Commands &GetCommands() { return m_Commands; }

    private:
        Commands m_Commands;

        std::shared_ptr<Image> m_SpaceBackground;
        std::shared_ptr<Body> m_Earth;
        std::shared_ptr<Body> m_Moon;
        std::shared_ptr<Orbit> m_Orbit;
        std::shared_ptr<Camera> m_Camera;

        float m_LastRenderTime = 0.0f;

        static std::vector<std::string> getBodyGUIItems(std::shared_ptr<Body> body);
        static std::vector<std::string> getOrbitGUIItems(std::shared_ptr<Ellipse> orbit);

    };
};
