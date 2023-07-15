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
    };

}
