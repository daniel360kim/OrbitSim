#pragma once

#include "Walnut/Layer.h"
#include "Scenes/OrbitViewer/OrbitViewer.h"

namespace Visualization
{
    class Application : public Walnut::Layer
    {
    public:
        Application();


        static std::shared_ptr<Application> Get();

        virtual void OnUpdate(float ts) override;
        virtual void OnUIRender() override;
    
    private:
        std::shared_ptr<Visualization::OrbitViewer> m_OrbitViewer;
    };
}