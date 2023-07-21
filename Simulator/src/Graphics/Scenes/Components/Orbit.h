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

namespace Visualization
{
    class Orbit : public Ellipse
    {
    public:
        Orbit(const OrbitalObject &orbitalObject, uint32_t orbitColor, uint32_t iconColor, uint32_t width, uint32_t height);

        void onResize(uint32_t width, uint32_t height);

        const std::shared_ptr<Body> &GetBody() const;
        void setOrbitColor(uint32_t color) { m_orbitColor = color; }
        void DrawOrbit(CameraInfo &cameraInfo, glm::vec2 &offset, std::vector<uint32_t> &pixels, SimulationTime::Time &simulationTime);
        void DrawIcon(CameraInfo &cameraInfo, std::vector<uint32_t> &pixels);

    private:
        uint32_t m_orbitColor;
        uint32_t m_iconColor;

        uint32_t m_Width;
        uint32_t m_Height;

        glm::vec2 m_iconOffset;

        std::shared_ptr<Circle> m_icon;

        void fillTriangle(int index1, int index2, int index3, uint32_t color1, uint32_t color2, uint32_t color3, std::vector<uint32_t> &pixels);

        uint32_t interpolateColor(uint32_t color1, uint32_t color2, float t);
        uint32_t interpolateComponent(uint32_t component1, uint32_t component2, float t);

        size_t findClosestPositionIndex(glm::vec3 position);
    };

} // namespace Visualization