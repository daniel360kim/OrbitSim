/**
 * @file Body.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief applies a texture to a sphere
 * @version 0.1
 * @date 2023-07-08
 *
 *
 */

#ifndef Body_H
#define Body_H

#include <vector>
#include <glm/glm.hpp>

#include "Shapes/Sphere.h"
#include "orbit/CentralBody.h"
#include "../Camera.h"

namespace Visualization
{
    class Body : public Sphere
    {
    public:
        Body(CentralBody &centralBody, int subdivisionlevel, const std::string &texturePath)
            : Sphere(centralBody.getRadius(), subdivisionlevel, texturePath), m_centralBody(centralBody) {}

        CentralBody GetCentralBody() const { return m_centralBody; }

        void Draw(CameraInfo &cameraInfo, glm::vec2 &offset, std::vector<uint32_t>& pixels);

        void onResize(uint32_t width, uint32_t height);

        float getScaleFactor() { return m_radius * m_Scaling; }

    private:
        CentralBody m_centralBody;
        float m_Scaling;

        uint32_t m_Width;
        uint32_t m_Height;

        template <typename T>
        struct Triangle
        {
            T v1, v2, v3; // three vertices
        };

        inline glm::vec2 transformToPixelCoords(glm::vec3 positionCoords, glm::vec2 &offset);
        void transformToPixelCoords(Triangle<glm::vec3> &trianglePositions, glm::vec2 &offset, Triangle<glm::vec2> &trianglePixels);

        uint32_t convertColors(const glm::vec4 &color);
        void fillTriangle(int index1, int index2, int index3, uint32_t color1, uint32_t color2, uint32_t color3, std::vector<uint32_t>& pixels);

        uint32_t interpolateColor(uint32_t color1, uint32_t color2, float t);
        uint32_t interpolateComponent(uint32_t component1, uint32_t component2, float t);

        inline bool isInFrontOfCamera(glm::vec3 &positionCoords, glm::vec3 &cameraDirection);
        bool isInFrontOfCamera(Triangle<glm::vec3> &positionCoords, glm::vec3 &cameraDirection);

        inline bool isWithinImageBounds(int x, int y);

        void applyTransformation(Triangle<glm::vec3> &trianglePositions, float yaw, float pitch, glm::vec3 position, float scale);
        void resetCameraScaling();
    };
}

#endif