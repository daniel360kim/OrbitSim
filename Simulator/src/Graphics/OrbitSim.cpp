
#include "Platform.h"

#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "Walnut/Random.h"

#include "Walnut/Image.h"

#include <memory>

#include "Application.h"

namespace Visualization
{
    static std::weak_ptr<Application> s_Application;

    std::shared_ptr<Application> Application::Get()
    {
        return s_Application.lock();
    }
}

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
    Walnut::ApplicationSpecification spec;
    spec.Name = "OrbitSim";

    Walnut::Application* app = new Walnut::Application(spec);

    std::shared_ptr<Visualization::Application> layer = std::make_shared<Visualization::Application>(app->GetFonts());
    app->PushLayer(layer);

    Visualization::s_Application = layer;

    return app;
}