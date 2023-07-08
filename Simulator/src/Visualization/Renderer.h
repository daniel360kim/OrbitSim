/**
 * @file Renderer.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief 
 * @version 0.1
 * @date 2023-07-04
 * 
 * 
 */

#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <memory>

#include "Walnut/Image.h"

#include "Camera.h"
#include "Texture.h"
#include "Image.h"
#include "Bodies/Body.h"

namespace Visualization
{
    class Renderer
    {
    public:
        Renderer(uint32_t width, uint32_t height);
        ~Renderer();

        void Draw(std::vector<std::shared_ptr<Body>>& bodies);

        void DrawBackground(std::shared_ptr<Image> background);
        void DrawBody(std::shared_ptr<Body> body, std::shared_ptr<Camera> camera, glm::vec2& offset);

        void Clear(uint32_t clearColor = 0xFF000000);
        void ClearRandom();

        void ResizeIfNeeded(uint32_t width, uint32_t height);

        void UpdateImage();

        uint32_t GetWidth() const { return m_Width; }
        uint32_t GetHeight() const { return m_Height; }

        std::vector<uint32_t>& GetImageBuffer() { return m_imageBuffer; }
        std::shared_ptr<Walnut::Image> GetImage() const { return m_Image; }
    
    private:
        std::shared_ptr<Walnut::Image> m_Image;
        std::vector<uint32_t> m_imageBuffer;
        uint32_t m_Width = 0;
        uint32_t m_Height = 0;

        std::vector<Body> m_Bodies;

        glm::vec2 transformToPixelCoords(glm::vec3 positionCoords, float scale, glm::vec2 offset);
        uint32_t convertColors(const glm::vec4& color);

        void fillTriangle(int index1, int index2, int index3, uint32_t color1, uint32_t color2, uint32_t color3);
        uint32_t interpolateColor(uint32_t color1, uint32_t color2, float t);
        uint32_t interpolateComponent(uint32_t component1, uint32_t component2, float t);

        inline bool isInFrontOfCamera(glm::vec3& positionCoords, glm::vec3& cameraDirection);
        bool isInFrontOfCamera(glm::vec3& positionCoords1, glm::vec3& positionCoords2, glm::vec3& positionCoords3,glm::vec3& cameraDirection);
        inline bool isWithinImageBounds(int x, int y);

    };
};










#endif