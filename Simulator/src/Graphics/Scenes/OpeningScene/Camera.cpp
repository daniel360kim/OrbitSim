/**
 * @file Camera.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief produces rotation, translation, and scaling matrices
 * @version 0.1
 * @date 2023-07-05
 *
 *
 */

#include "Camera.h"

#include <Walnut/Input/Input.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Visualization
{
    namespace OpeningSceneScene
    {
        Camera::Camera()
            : m_position(0.0f), m_scale(1.0f),
              m_yaw(0.0f), m_pitch(0.0f)
        {
            m_forwardDirection = glm::vec3(0.0f, 0.0f, -1.0f);
            UpdateViewMatrix();
        }

        glm::vec3 Camera::getForwardDirection() const
        {
            return m_forwardDirection;
        }

        glm::vec3 Camera::getRightDirection() const
        {
            glm::vec3 right = glm::cross(getForwardDirection(), getUpDirection());
            return right;
        }

        glm::vec3 Camera::getUpDirection() const
        {
            constexpr glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
            return up;
        }

        bool Camera::OnUpdate(float ts)
        {
            bool moved = false;

            float rotationSpeed = 0.02f;

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

        void Camera::SetPosition(const glm::vec3 &newPosition)
        {
            m_position = newPosition;
            UpdateViewMatrix();
        }

        void Camera::SetScale(float newScale)
        {
            m_scale = newScale;
            UpdateViewMatrix();
        }

        void Camera::SetYaw(float newYaw)
        {
            m_yaw = newYaw;
            UpdateViewMatrix();
        }

        void Camera::SetPitch(float newPitch)
        {
            m_pitch = newPitch;
            UpdateViewMatrix();
        }

        glm::mat4 Camera::GetViewMatrix() const
        {
            return m_viewMatrix;
        }

        CameraInfo Camera::GetCameraInfo() const
        {
            CameraInfo info;
            info.position = m_position;
            info.scale = m_scale;
            info.forwardDirection = getForwardDirection();
            return info;
        }

        void Camera::UpdateViewMatrix()
        {
            // Update view matrix based on position, yaw, and pitch
            glm::mat4 rotation = glm::mat4(1.0f);
            rotation = glm::rotate(rotation, glm::radians(m_yaw), glm::vec3(0.0f, 1.0f, 0.0f));
            rotation = glm::rotate(rotation, glm::radians(m_pitch), glm::vec3(1.0f, 0.0f, 0.0f));

            glm::mat4 translation = glm::mat4(1.0f);
            translation = glm::translate(translation, -m_position);

            glm::mat4 scale = glm::mat4(1.0f);
            scale = glm::scale(scale, glm::vec3(m_scale));

            m_viewMatrix = rotation * translation * scale;
        }
    }
}