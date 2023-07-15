/**
 * @file OpeningScene.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief
 * @version 0.1
 * @date 2023-07-14
 *
 *
 */

#pragma once

#include "../Scene.h"
#include "Camera.h"
#include "../Components/Image.h"
#include <glm/glm.hpp>
#include "../Components/Bodies.h"

namespace Visualization
{
    struct Commands
    {
        bool m_Exit = false;
    };

    class OpeningScene : public Scene
    {
    public:
        OpeningScene(uint32_t width, uint32_t height);
        ~OpeningScene();

        void Draw();
        void DrawBackground();
        void DrawBody(std::shared_ptr<CentralRenderBody> body, std::shared_ptr<OpeningSceneScene::Camera> camera, glm::vec2 &offset);

        void OnUpdate(float ts);
        void OnUIRender();

        void ResizeIfNeeded(uint32_t width, uint32_t height) override;

        Commands &GetCommands() { return m_Commands; }

    private:
        Commands m_Commands;

        std::shared_ptr<Walnut::Image> m_StartButtonIcon;

        std::shared_ptr<Image> m_SpaceBackground;

        std::shared_ptr<CentralRenderBody> m_Mars;

        std::shared_ptr<OpeningSceneScene::Camera> m_Camera;

        template <typename T>
        struct Triangle
        {
            T v1, v2, v3; // three vertices
        };

        inline glm::vec2 transformToPixelCoords(glm::vec3 positionCoords, float scale, glm::vec2 &offset);
        void transformToPixelCoords(Triangle<glm::vec3> &trianglePositions, float scale, glm::vec2 &offset, Triangle<glm::vec2> &trianglePixels);
        uint32_t convertColors(const glm::vec4 &color);

        void fillTriangle(int index1, int index2, int index3, uint32_t color1, uint32_t color2, uint32_t color3);
        uint32_t interpolateColor(uint32_t color1, uint32_t color2, float t);
        uint32_t interpolateComponent(uint32_t component1, uint32_t component2, float t);

        inline bool isInFrontOfCamera(glm::vec3 &positionCoords, glm::vec3 &cameraDirection);
        bool isInFrontOfCamera(Triangle<glm::vec3> &positionCoords, glm::vec3 &cameraDirection);
        inline bool isWithinImageBounds(int x, int y);

        void applyTransformation(Triangle<glm::vec3> &trianglePositions, float yaw, float pitch, glm::vec3 position, float scale);

        void resetCameraScaling();

        float m_Scaling = 1.0f;
    };

}