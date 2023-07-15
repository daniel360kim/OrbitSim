/**
 * @file OpeningSceneCamera.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief 
 * @version 0.1
 * @date 2023-07-15
 * 
 * 
 */

#pragma once

#include "../Camera.h"

namespace Visualization
{
    class OpeningSceneCamera : public Camera
    {
    public:
        OpeningSceneCamera() : Camera() {}

        virtual bool OnUpdate(float ts) override;
    };
};