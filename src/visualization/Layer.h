/**
 * @file Layer.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief layer of the application
 * @version 0.1
 * @date 2023-06-24
 *
 *
 */

#ifndef LAYER_H
#define LAYER_H

#include "Events/Event.h"
#include "Timestep.h"

//Gets subclassed to make your own layer
class Layer
{
public:
    Layer(const std::string &name = "Layer") : m_DebugName(name) {}
    virtual ~Layer() = default;

    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate(Timestep ts) {}
    virtual void OnImGuiRender() {}
    virtual void OnEvent(Event &event) {}

    const std::string &GetName() const { return m_DebugName; }

protected:
    std::string m_DebugName;
};

#endif