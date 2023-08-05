

#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "Walnut/Random.h"

#include "Walnut/Image.h"

#include <memory>

#include "Application.h"
#include "Scenes/SceneSelection.h"

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
    	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}

        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Earth Orbit Viewer"))
            {
                Visualization::Application::Get()->SetScene(SceneSelection::EarthOrbitViewer);
            }
            if (ImGui::MenuItem("Satellite Search"))
            {
                Visualization::Application::Get()->SetScene(SceneSelection::SatelliteSearch);
            }
            ImGui::EndMenu();
        }
	});


    Visualization::s_Application = layer;

    return app;
}