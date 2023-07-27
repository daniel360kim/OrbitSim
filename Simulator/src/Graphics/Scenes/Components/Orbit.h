/**
 * @file Orbits.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief
 * @version 0.1
 * @date 2023-07-16
 *
 *
 */

#pragma once

#include "orbit/OrbitalObject.h"
#include "Shapes/Circle.h"
#include "Shapes/Ellipse.h"
#include "Bodies.h"
#include "util/ColorConversion.h"

namespace Visualization
{
    class Orbit : public Ellipse
    {
    public:
        Orbit(const OrbitalObject &orbitalObject, glm::vec4 orbitColor, glm::vec4 iconColor, uint32_t width, uint32_t height);

        void onResize(uint32_t width, uint32_t height);

        void DrawOrbit(CameraInfo &cameraInfo, glm::vec2 &offset, std::vector<uint32_t> &pixels, SimulationTime::Time &simulationTime);
        void DrawIcon(CameraInfo &cameraInfo, std::vector<uint32_t> &pixels);

        void setOrbitColor(glm::vec4 color) { m_orbitColor = color; }
        void setIconColor(glm::vec4 color) { m_iconColor = color; }

    private:
        glm::vec4 m_orbitColor;
        glm::vec4 m_iconColor;

        uint32_t m_Width;
        uint32_t m_Height;

        glm::vec2 m_iconOffset;

        std::shared_ptr<Circle> m_icon;

        size_t findClosestPositionIndex(glm::vec3 position);
    };

} // namespace Visualization