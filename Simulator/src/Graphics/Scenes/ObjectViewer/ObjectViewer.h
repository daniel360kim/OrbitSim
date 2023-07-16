/**
 * @file ObjectViewer.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief
 * @version 0.1
 * @date 2023-07-14
 *
 *
 */

#pragma once

#include "../Scene.h"
#include "../Camera.h"
#include "../Components/Image.h"
#include "../Components/Bodies.h"

namespace Visualization
{
    class ObjectViewer : public Scene
    {
    public:
        ObjectViewer(uint32_t width, uint32_t height);
        ~ObjectViewer();

        void OnUpdate(float ts);
        void OnUIRender();

        void Draw();
    private:
        std::shared_ptr<Image> m_SpaceBackground;
                
        std::vector<std::shared_ptr<CentralRenderBody>> m_Bodies;

        std::shared_ptr<Camera> m_Camera;
    };

}
