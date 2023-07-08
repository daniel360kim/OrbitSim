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
    Camera::Camera(float screenWidth, float screenHeight)
        : m_screenWidth(screenWidth), m_screenHeight(screenHeight),
          m_position(0.0f), m_scale(1.0f),
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
        using namespace Walnut;
        glm::vec2 mousePosition = Input::GetMousePosition();
        glm::vec2 deltaMousePosition = (mousePosition - m_lastMousePosition) * 0.002f;
        m_lastMousePosition = mousePosition;

        if (!Input::IsMouseButtonDown(MouseButton::Right))
        {
            Input::SetCursorMode(CursorMode::Normal);
            return false;
        }

        Input::SetCursorMode(CursorMode::Locked);

        bool moved = false;

        float movementSpeed = 0.5f;

        glm::vec3 forward = getForwardDirection();
        glm::vec3 right = getRightDirection();
        glm::vec3 up = getUpDirection();

        if (Input::IsKeyDown(KeyCode::W))
        {
            m_position -= up * movementSpeed * ts;
            moved = true;
        }
        else if (Input::IsKeyDown(KeyCode::S))
        {
            m_position += up * movementSpeed * ts;
            moved = true;
        }
        if (Input::IsKeyDown(KeyCode::A))
        {
            m_position -= right * movementSpeed * ts;
            moved = true;
        }
        else if (Input::IsKeyDown(KeyCode::D))
        {
            m_position += right * movementSpeed * ts;
            moved = true;
        }

        float zoomSpeed = 0.5f;
        // Q and E for scaling
        if (Input::IsKeyDown(KeyCode::Q))
        {
            m_scale -= zoomSpeed * ts;
            moved = true;
        }
        else if (Input::IsKeyDown(KeyCode::E))
        {
            m_scale += zoomSpeed * ts;
            moved = true;
        }

        // TODO add mouse sensitivity setting
        if (deltaMousePosition.x != 0.0f || deltaMousePosition.y != 0.0f)
        {
            // Calculate new yaw and pitch
            m_yaw += deltaMousePosition.x;
            m_pitch += deltaMousePosition.y;

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
        }

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