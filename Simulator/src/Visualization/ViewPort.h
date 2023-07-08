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
#include "Sphere.h"
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

        std::shared_ptr<Visualization::Sphere> GetSphere() const { return m_Sphere; }
        std::shared_ptr<Visualization::Camera> GetCamera() const { return m_Camera; }

    private:
        std::shared_ptr<Visualization::Sphere> m_Sphere;
        std::shared_ptr<Visualization::Renderer> m_Renderer;
        std::shared_ptr<Visualization::Camera> m_Camera;
    };

}

#endif