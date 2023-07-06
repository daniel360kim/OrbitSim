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
#include "Body/Sphere.h"
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

        std::shared_ptr<Visualization::Image> GetEarth() const { return m_Earth; }
        std::shared_ptr<Visualization::Sphere> GetSphere() const { return m_Sphere; }

    private:
        std::shared_ptr<Visualization::Image> m_Earth;
        std::shared_ptr<Visualization::Sphere> m_Sphere;
        std::shared_ptr<Visualization::Renderer> m_Renderer;

        Camera m_Camera;

        float scale;
    };

}

#endif