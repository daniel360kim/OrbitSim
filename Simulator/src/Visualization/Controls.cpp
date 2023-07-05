/**
 * @file Controls.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief 
 * @version 0.1
 * @date 2023-07-04
 * 
 * 
 */

#include "Controls.h"

#include <imgui.h>

namespace Visualization
{
    Controls::Controls()
    {
    }

    Controls::~Controls()
    {
    }

    void Controls::OnUpdate(float ts)
    {
    }

    void Controls::OnUIRender()
    {
        ImGui::Begin("Controls");

        ImGui::End();

        
    }
}