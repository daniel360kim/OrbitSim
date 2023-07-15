#pragma once

#include <unordered_map>

#include "Walnut/Layer.h"
#include "Scenes/OpeningScene/OpeningScene.h"
#include "Scenes/OrbitViewer/OrbitViewer.h"
#include "Scenes/ObjectViewer/ObjectViewer.h"

namespace Visualization
{
    class Application : public Walnut::Layer
    {
    public:
        enum class Scene
        {
            OpeningScene,
            OrbitViewer,
            ObjectViewer
        };

        Application();

        static std::shared_ptr<Application> Get();

        virtual void OnUpdate(float ts) override;
        virtual void OnUIRender() override;
    
    private:
        Scene m_CurrentScene;
        std::shared_ptr<OpeningScene> m_OpeningScene;
        std::shared_ptr<OrbitViewer> m_OrbitViewer;
        std::shared_ptr<ObjectViewer> m_ObjectViewer;
    };
}