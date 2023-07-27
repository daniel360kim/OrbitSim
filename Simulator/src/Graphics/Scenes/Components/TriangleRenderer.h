/**
 * @file TriangleRenderer.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief 
 * @version 0.1
 * @date 2023-07-21
 * 
 * 
 */

#pragma once

#include <vector>
#include <stdexcept>
#include <glm/glm.hpp>

#include "util/ColorConversion.h"

namespace Visualization
{
    template <typename T>
    struct Triangle
    {
        T v1, v2, v3; // three vertices
    };
    
    class TriangleRenderer
    {
    public:
        TriangleRenderer(uint32_t width, uint32_t height) : m_Width(width), m_Height(height) {}

        void fillTriangle(Triangle<int> indices, Triangle<uint32_t> colors, std::vector<uint32_t> &pixels);
        bool isWithinBounds(int x, int y);
        bool isWithinBounds(Triangle<int> x, Triangle<int> y);

        void getArrayIndices(Triangle<int>& x, Triangle<int>&y, Triangle<int>& indices, Triangle<glm::vec2> pixelCoords);

        void applyTransformation(Triangle<glm::vec3>& trianglePositions, float yaw, float pitch, glm::vec3 position, float scale);
        bool isInFrontOfCamera(glm::vec3 position, glm::vec3 cameraForwardDirection);
        bool isInFrontOfCamera(Triangle<glm::vec3> trianglePositions, glm::vec3 cameraForwardDirection);

        glm::vec2 transformToPixelCoords(glm::vec3 positionCoords);
        void transformToPixelCoords(Triangle<glm::vec3> trianglePositions, Triangle<glm::vec2>& trianglePixels);

        void colorToInt(Triangle<glm::vec4> colors, Triangle<uint32_t>& intColors);

    private:
        uint32_t m_Width;
        uint32_t m_Height;
        
        inline uint32_t interpolateComponent(uint32_t component1, uint32_t component2, float t);
        uint32_t interpolateColor(uint32_t color1, uint32_t color2, float t);

    };
}