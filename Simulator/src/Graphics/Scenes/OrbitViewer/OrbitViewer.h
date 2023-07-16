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
#include "../Components/Shapes/Ellipse.h"

#include "../Camera.h"

namespace Visualization
{
    class OrbitViewer : public Scene
    {
    public:
        OrbitViewer(uint32_t width, uint32_t height);
        ~OrbitViewer();

        void OnUpdate(float ts);
        void OnUIRender(std::vector<ImFont*>& fonts);

        void ResizeIfNeeded(uint32_t width, uint32_t height) override;

        void Draw();
        void DrawBackground(std::shared_ptr<Image> background);

    private:
        std::shared_ptr<Image> m_SpaceBackground;
        std::shared_ptr<CentralRenderBody> m_Earth;
        std::shared_ptr<Ellipse> m_Orbit;
        std::shared_ptr<Camera> m_Camera;
    };
};

