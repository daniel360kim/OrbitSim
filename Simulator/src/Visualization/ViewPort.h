/**
 * @file ViewPort.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief
 * @version 0.1
 * @date 2023-07-04
 *
 *
 */

#ifndef ViewPort_H
#define ViewPort_H

#include "Walnut/Layer.h"

#include "Renderer.h"
#include "Image.h"
#include "Bodies/Body.h"
#include "Camera.h"

#include <memory>

namespace Visualization
{
    class ViewPort : public Walnut::Layer
    {
    public:
        ViewPort();

        ~ViewPort();

        virtual void OnUpdate(float ts) override;
        virtual void OnUIRender() override;

        static std::shared_ptr<ViewPort> Get();

        std::shared_ptr<Visualization::Body> GetEarth() const { return m_Earth; }
        std::shared_ptr<Visualization::Camera> GetCamera() const { return m_Camera; }
        std::shared_ptr<Visualization::Image> GetSpaceBackground() const { return m_SpaceBackground; }

        std::vector<std::shared_ptr<Visualization::Body>> GetBodies() const { return m_Bodies; }


    private:
        std::shared_ptr<Visualization::Image> m_SpaceBackground;
        std::shared_ptr<Visualization::Body> m_Earth;
        std::shared_ptr<Visualization::Body> m_Sun;
        std::shared_ptr<Visualization::Body> m_Moon;

        std::vector<std::shared_ptr<Visualization::Body>> m_Bodies;
        
        std::shared_ptr<Visualization::Renderer> m_Renderer;
        std::shared_ptr<Visualization::Camera> m_Camera;
    };

}

#endif