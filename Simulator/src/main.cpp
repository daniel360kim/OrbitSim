#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "Walnut/Random.h"

#include "Walnut/Image.h"

#include <memory>

#include "Visualization/Controls.h"
#include "Visualization/ViewPort.h"

namespace Visualization
{
    static std::weak_ptr<ViewPort> s_ViewPort;

    std::shared_ptr<ViewPort> ViewPort::Get()
    {
        return s_ViewPort.lock();
    }

    static std::weak_ptr<Controls> s_Controls;

    std::shared_ptr<Controls> Controls::Get()
    {
        return s_Controls.lock();
    }
}

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "OrbitSim";

	Walnut::Application* app = new Walnut::Application(spec);
    
    std::shared_ptr<Visualization::Controls> controls = std::make_shared<Visualization::Controls>();
    std::shared_ptr<Visualization::ViewPort> layer = std::make_shared<Visualization::ViewPort>();
	app->PushLayer(layer);
    app->PushLayer(controls);

    Visualization::s_ViewPort = layer;
    Visualization::s_Controls = controls;
    
	return app;
} 