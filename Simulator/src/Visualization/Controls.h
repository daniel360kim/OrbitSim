/**
 * @file Controls.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief 
 * @version 0.1
 * @date 2023-07-04
 * 
 * 
 */

#ifndef CONTROLS_H
#define CONTROLS_H

#include "Walnut/Layer.h"

#include <memory>

namespace Visualization
{
    class Controls : public Walnut::Layer
    {
    public: 
        Controls();

        ~Controls();

        virtual void OnUpdate(float ts) override;
        virtual void OnUIRender() override;

        static std::shared_ptr<Controls> Get();

        
    private:
    };
}




#endif