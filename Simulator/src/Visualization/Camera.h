/**
 * @file Camera.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief produces rotation, translation, and scaling matrices
 * @version 0.1
 * @date 2023-07-05
 *
 *
 */

#ifndef Camera_H
#define Camera_H

#include <glm/glm.hpp>

namespace Visualization
{ 
    class Camera
    {
    public:
        Camera(float screenWidth, float screenHeight);

        bool OnUpdate(float ts);

        void SetPosition(const glm::vec3 &newPosition);
        void SetScale(float newScale);

        void SetYaw(float newYaw);
        void SetPitch(float newPitch);

        const glm::vec3 &GetPosition() const { return m_position; }
        float GetYaw() const { return m_yaw; }
        float GetPitch() const { return m_pitch; }
        float GetScale() const { return m_scale; }

        glm::mat4 GetViewMatrix() const;

        glm::vec3 getForwardDirection() const;
        glm::vec3 getRightDirection() const;
        glm::vec3 getUpDirection() const;

    private:
        float m_screenWidth;
        float m_screenHeight;

        glm::vec3 m_position;
        glm::vec3 m_forwardDirection; // Needed so that up direction doesn't change when rotating
        float m_scale;

        float m_yaw;   // Rotation around the y-axis (yaw angle)
        float m_pitch; // Rotation around the x-axis (pitch angle)

        glm::mat4 m_viewMatrix;

        glm::vec2 m_lastMousePosition;
        
        float m_yTranslationElapsed; // Used for 
        void UpdateViewMatrix();
    };
}

#endif