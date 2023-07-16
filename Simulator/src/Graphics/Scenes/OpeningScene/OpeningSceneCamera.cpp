/**
 * @file OpeningSceneCamera.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief
 * @version 0.1
 * @date 2023-07-15
 *
 *
 */

#include "OpeningSceneCamera.h"

namespace Visualization
{
    bool OpeningSceneCamera::OnUpdate(float ts)
    {
        bool moved = false;

        float rotationSpeed = 0.1f;

        glm::vec2 delta = glm::vec2(1.0f, 0.0f) * ts;

        // Calculate new yaw and pitch
        m_yaw += -delta.x * rotationSpeed;
        m_pitch += -delta.y * rotationSpeed;

        // Clamp pitch
        if (m_pitch > 89.0f)
        {
            m_pitch = 89.0f;
        }
        else if (m_pitch < -89.0f)
        {
            m_pitch = -89.0f;
        }

        moved = true;

        if (moved)
        {
            UpdateViewMatrix();
        }

        return moved;
    }
}