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
#include "OpeningSceneCamera.h"
#include "../Components/Image.h"
#include <glm/glm.hpp>
#include "../Components/Bodies.h"

namespace Visualization
{
    class OpeningScene : public Scene
    {
    public:
        struct Commands
        {
            bool m_Exit = false;
        };

        OpeningScene(uint32_t width, uint32_t height);
        ~OpeningScene();

        void Draw();
        void DrawBackground();

        void OnUpdate(float ts);
        void OnUIRender(std::vector<ImFont *> &fonts);

        void ResizeIfNeeded(uint32_t width, uint32_t height) override;

        Commands &GetCommands() { return m_Commands; }

    private:
        Commands m_Commands;

        std::shared_ptr<Walnut::Image> m_StartButtonIcon;
        std::shared_ptr<Image> m_SpaceBackground;
        std::shared_ptr<Body> m_Mars;
        std::shared_ptr<OpeningSceneCamera> m_Camera;

        float m_LastRenderTime = 0.0f;
    };

}