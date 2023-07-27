#pragma once

#include <unordered_map>

#include "Walnut/Layer.h"
#include "Scenes/OpeningScene/OpeningScene.h"
#include "Scenes/EarthOrbitViewer/EarthOrbitViewer.h"
#include "SimulationTime.h"
#include "Scenes/ObjectViewer/ObjectViewer.h"
#include "Scenes/SatelliteSearch/SatelliteSearch.h"
#include "Scenes/SceneSelection.h"

namespace Visualization
{
    class Application : public Walnut::Layer
    {
    public:

        Application(std::vector<ImFont*> fonts);

        static std::shared_ptr<Application> Get();

        virtual void OnUpdate(float ts) override;
        virtual void OnUIRender() override;
    
    private:
        SceneSelection m_CurrentScene;
        SimulationTime m_SimulationTime;
        std::shared_ptr<OpeningScene> m_OpeningScene;
        std::shared_ptr<EarthOrbitViewer> m_EarthOrbitViewer;
        std::shared_ptr<ObjectViewer> m_ObjectViewer;
        std::shared_ptr<SatelliteSearch> m_SatelliteSearch;

        std::shared_ptr<std::vector<ImFont*>> m_Fonts;
    };
}