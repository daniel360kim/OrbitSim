/**
 * @file OpeningScene.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief
 * @version 0.1
 * @date 2023-07-14
 *
 *
 */

#include "OpeningScene.h"
#include "orbit/CentralBody.h"
#include "../Components/ImageResizer.h"

#include <imgui.h>
#include <glm/gtx/rotate_vector.hpp>

namespace Visualization
{
    OpeningScene::OpeningScene(uint32_t width, uint32_t height)
        : Scene(width, height)
    {
        std::vector<uint32_t> rocketIconImageBuffer(20 * 20);
        ImageResizer::resizeImage("../../Resources/Icons/rocket.png", 20, 20, rocketIconImageBuffer);

        m_StartButtonIcon = std::make_shared<Walnut::Image>(20, 20, Walnut::ImageFormat::RGBA, rocketIconImageBuffer.data());

        m_SpaceBackground = std::make_shared<Image>("../../Resources/Textures/milkyway.jpg");
        m_Camera = std::make_shared<OpeningSceneScene::Camera>();

        CentralBody mars("mars", Type::Planet, 6.4171e23, 3389.5, 3);

        m_Mars = std::make_shared<Visualization::CentralRenderBody>(mars, 6, "../../Resources/Textures/mars.jpg");
    }

    OpeningScene::~OpeningScene()
    {
    }

    void OpeningScene::OnUpdate(float ts)
    {
        m_Camera->OnUpdate(ts);
        Clear();
        Draw();
        UpdateImage();
    }

    void OpeningScene::OnUIRender()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
        ImGui::Begin("OrbitSim");

        uint32_t windowWidth = (uint32_t)ImGui::GetWindowWidth();
        uint32_t windowHeight = (uint32_t)ImGui::GetWindowHeight();

        ImGui::Image(GetImage()->GetDescriptorSet(), ImVec2((float)GetWidth(), (float)GetHeight()));

        // Set cursor to the third of the window width from the right and middle of the window height
        ImGui::SetCursorPos(ImVec2(windowWidth / 3 + windowWidth / 3, windowHeight / 2));

        // Save the current button color and hover color
        ImVec4 defaultButtonColor = ImGui::GetStyleColorVec4(ImGuiCol_Button);
        ImVec4 defaultHoverColor = ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered);
        ImVec4 defaultBackgroundColor = ImGui::GetStyleColorVec4(ImGuiCol_WindowBg);

        // Set custom button and hover colors
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.369f, 0.157f, 0.969f, 1.0f));

        // Set custom button border properties
        ImGuiStyle &style = ImGui::GetStyle();
        style.FrameBorderSize = 1.0f; // Set border thickness
        style.Colors[ImGuiCol_Border] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

        style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

        if (ImGui::Button("Start", ImVec2(150, 75)))
        {
            m_Commands.m_Exit = true;
        }
        // Restore the default button and hover colors
        ImGui::PopStyleColor(2);

        // Restore the default button border properties
        style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        style.FrameBorderSize = 0.0f;
        style.Colors[ImGuiCol_Border] = ImVec4(0, 0, 0, 0);

        ResizeIfNeeded(windowWidth, windowHeight);


        ImGui::End();

        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
    }

    void OpeningScene::ResizeIfNeeded(uint32_t width, uint32_t height)
    {
        if (m_Width == width && m_Height == height)
            return;

        m_Width = width;
        m_Height = height;

        m_Image = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);

        m_imageBuffer.resize(width * height);

        resetCameraScaling();
    }

    void OpeningScene::Draw()
    {
        DrawBackground();

        // Set offset so the body is in the first third of the screen (from the left) and in the middle of the screen (from the top)
        glm::vec2 offset = glm::vec2(m_Width / 5, m_Height + (m_Height / 6));
        DrawBody(m_Mars, m_Camera, offset);
    }

    void OpeningScene::DrawBackground()
    {
        // Draw the space background
        uint32_t *backgroundData = m_SpaceBackground->GetPixels();
        uint32_t backgroundWidth = m_SpaceBackground->GetWidth();
        uint32_t backgroundHeight = m_SpaceBackground->GetHeight();

        // Add to image buffer. Fit only the center of the image if the image is bigger than the screen
        for (uint32_t y = 0; y < m_Height; y++)
        {
            for (uint32_t x = 0; x < m_Width; x++)
            {
                m_imageBuffer[y * m_Width + x] = backgroundData[(y % backgroundHeight) * backgroundWidth + (x % backgroundWidth)];
            }
        }
    }

    void OpeningScene::DrawBody(std::shared_ptr<CentralRenderBody> body, std::shared_ptr<OpeningSceneScene::Camera> camera, glm::vec2 &offset)
    {
        const std::vector<unsigned int> &indices = body->GetIndices();
        const std::vector<glm::vec3> &positions = body->GetPositions();
        const std::vector<glm::vec2> &texCoords = body->GetTexCoords();

        OpeningSceneScene::CameraInfo cameraInfo = camera->GetCameraInfo();

        // #pragma omp parallel for
        for (uint32_t i = 0; i < indices.size(); i += 3)
        {
            Triangle<glm::vec3> trianglePosition;
            trianglePosition = {positions[indices[i]], positions[indices[i + 1]], positions[indices[i + 2]]};

            // Apply transformations using the Camera properties
            applyTransformation(trianglePosition, camera->GetYaw(), camera->GetPitch(), camera->GetPosition(), body->GetRadius() * m_Scaling);

            // Check if the positions are in front of the camera - only render what the camera can see
            Triangle<glm::vec3> cameraPositionDistance;
            cameraPositionDistance.v1 = trianglePosition.v1 - cameraInfo.position;
            cameraPositionDistance.v2 = trianglePosition.v2 - cameraInfo.position;
            cameraPositionDistance.v3 = trianglePosition.v3 - cameraInfo.position;

            if (isInFrontOfCamera(cameraPositionDistance, cameraInfo.forwardDirection))
            {
                // Transform to pixel coordinates
                Triangle<glm::vec2> pixelCoords;
                transformToPixelCoords(trianglePosition, cameraInfo.scale, offset, pixelCoords);

                // Convert the pixel coordinates to array indices
                int x1 = static_cast<int>(pixelCoords.v1.x);
                int y1 = static_cast<int>(pixelCoords.v1.y);
                int index1 = y1 * m_Width + x1;

                int x2 = static_cast<int>(pixelCoords.v2.x);
                int y2 = static_cast<int>(pixelCoords.v2.y);
                int index2 = y2 * m_Width + x2;

                int x3 = static_cast<int>(pixelCoords.v3.x);
                int y3 = static_cast<int>(pixelCoords.v3.y);
                int index3 = y3 * m_Width + x3;

                // Check if the pixels are within the bounds of the m_Image image
                if (isWithinImageBounds(x1, y1) &&
                    isWithinImageBounds(x2, y2) &&
                    isWithinImageBounds(x3, y3))
                {
                    // Map texture coordinates to the triangle vertices
                    glm::vec2 texCoords1 = texCoords[indices[i]];
                    glm::vec2 texCoords2 = texCoords[indices[i + 1]];
                    glm::vec2 texCoords3 = texCoords[indices[i + 2]];

                    // Sample the texture colors
                    glm::vec4 color1 = body->GetTexture()->getPixel(glm::vec2(texCoords1.x, texCoords1.y));
                    glm::vec4 color2 = body->GetTexture()->getPixel(glm::vec2(texCoords2.x, texCoords2.y));
                    glm::vec4 color3 = body->GetTexture()->getPixel(glm::vec2(texCoords3.x, texCoords3.y));

                    // Convert the colors to 32-bit integers
                    uint32_t colorInt1 = convertColors(color1);
                    uint32_t colorInt2 = convertColors(color2);
                    uint32_t colorInt3 = convertColors(color3);

                    // Fill the triangle with the corresponding colors
                    fillTriangle(index1, index2, index3, colorInt1, colorInt2, colorInt3);
                }
            }
        }
    }

    glm::vec2 OpeningScene::transformToPixelCoords(glm::vec3 positionCoords, float scale, glm::vec2 &offset)
    {
        return glm::vec2(positionCoords.x, positionCoords.y) * scale + offset;
    }

    void OpeningScene::transformToPixelCoords(Triangle<glm::vec3> &triangle, float scale, glm::vec2 &offset, Triangle<glm::vec2> &trianglePixelCoords)
    {
        trianglePixelCoords.v1 = transformToPixelCoords(triangle.v1, scale, offset);
        trianglePixelCoords.v2 = transformToPixelCoords(triangle.v2, scale, offset);
        trianglePixelCoords.v3 = transformToPixelCoords(triangle.v3, scale, offset);
    }

    uint32_t OpeningScene::convertColors(const glm::vec4 &color)
    {
        uint32_t colorInt = (static_cast<uint32_t>(color.a * 255.0f) << 24) |
                            (static_cast<uint32_t>(color.b * 255.0f) << 16) |
                            (static_cast<uint32_t>(color.g * 255.0f) << 8) |
                            (static_cast<uint32_t>(color.r * 255.0f));

        return colorInt;
    }

    void OpeningScene::fillTriangle(int index1, int index2, int index3, uint32_t color1, uint32_t color2, uint32_t color3)
    {
        // Sort the vertices vertically by y-coordinate (top to bottom)
        if (index2 < index1)
        {
            std::swap(index1, index2);
            std::swap(color1, color2);
        }
        if (index3 < index2)
        {
            std::swap(index2, index3);
            std::swap(color2, color3);
        }
        if (index2 < index1)
        {
            std::swap(index1, index2);
            std::swap(color1, color2);
        }

        int x1 = index1 % m_Width;
        int y1 = index1 / m_Width;

        int x2 = index2 % m_Width;
        int y2 = index2 / m_Width;

        int x3 = index3 % m_Width;
        int y3 = index3 / m_Width;

        // Calculate the slopes of the edges
        float slope1 = static_cast<float>(x2 - x1) / (y2 - y1);
        float slope2 = static_cast<float>(x3 - x1) / (y3 - y1);
        float slope3 = static_cast<float>(x3 - x2) / (y3 - y2);

        // Draw the triangle scanlines
        for (int y = y1; y <= y3; y++)
        {
            int startX = static_cast<int>(x1 + (y - y1) * slope1);
            int endX = static_cast<int>(x1 + (y - y1) * slope2);

            if (y > y2)
                startX = static_cast<int>(x2 + (y - y2) * slope3);

            if (startX > endX)
                std::swap(startX, endX);

            // Clip the scanline to the image boundaries
            startX = std::max<uint32_t>(0, std::min<uint32_t>(m_Width - 1, startX));
            endX = std::max<uint32_t>(0, std::min<uint32_t>(m_Width - 1, endX));

            int currentIndex = y * m_Width + startX;
            for (int x = startX; x <= endX; x++)
            {
                // Linearly interpolate the color components
                float t = static_cast<float>(x - startX) / (endX - startX);
                uint32_t color = interpolateColor(color1, color3, t);

                // Set the pixel color in the image buffer
                m_imageBuffer[currentIndex] = color;

                currentIndex++;
            }
        }
    }

    uint32_t OpeningScene::interpolateColor(uint32_t color1, uint32_t color2, float t)
    {
        uint32_t a1 = (color1 >> 24) & 0xFF;
        uint32_t r1 = (color1 >> 16) & 0xFF;
        uint32_t g1 = (color1 >> 8) & 0xFF;
        uint32_t b1 = color1 & 0xFF;

        uint32_t a2 = (color2 >> 24) & 0xFF;
        uint32_t r2 = (color2 >> 16) & 0xFF;
        uint32_t g2 = (color2 >> 8) & 0xFF;
        uint32_t b2 = color2 & 0xFF;

        uint32_t a = interpolateComponent(a1, a2, t);
        uint32_t r = interpolateComponent(r1, r2, t);
        uint32_t g = interpolateComponent(g1, g2, t);
        uint32_t b = interpolateComponent(b1, b2, t);

        return (a << 24) | (r << 16) | (g << 8) | b;
    }

    uint32_t OpeningScene::interpolateComponent(uint32_t component1, uint32_t component2, float t)
    {
        return static_cast<uint32_t>((1.0f - t) * component1 + t * component2);
    }

    bool OpeningScene::isInFrontOfCamera(glm::vec3 &positionCoords, glm::vec3 &cameraDirection)
    {
        return glm::dot(positionCoords, cameraDirection) > 0.0f;
    }

    bool OpeningScene::isInFrontOfCamera(Triangle<glm::vec3> &positionCoords, glm::vec3 &cameraDirection)
    {
        return isInFrontOfCamera(positionCoords.v1, cameraDirection) &&
               isInFrontOfCamera(positionCoords.v2, cameraDirection) &&
               isInFrontOfCamera(positionCoords.v3, cameraDirection);
    }

    bool OpeningScene::isWithinImageBounds(int x, int y)
    {
        return x >= 0 && x < m_Width && y >= 0 && y < m_Height;
    }

    void OpeningScene::applyTransformation(Triangle<glm::vec3> &trianglePositions, float yaw, float pitch, glm::vec3 position, float scale)
    {
        trianglePositions.v1 *= scale;
        trianglePositions.v2 *= scale;
        trianglePositions.v3 *= scale;

        trianglePositions.v1 = glm::rotateX(trianglePositions.v1, pitch);
        trianglePositions.v2 = glm::rotateX(trianglePositions.v2, pitch);
        trianglePositions.v3 = glm::rotateX(trianglePositions.v3, pitch);

        trianglePositions.v1 = glm::rotateY(trianglePositions.v1, yaw);
        trianglePositions.v2 = glm::rotateY(trianglePositions.v2, yaw);
        trianglePositions.v3 = glm::rotateY(trianglePositions.v3, yaw);

        trianglePositions.v1 += position;
        trianglePositions.v2 += position;
        trianglePositions.v3 += position;
    }

    void OpeningScene::resetCameraScaling()
    {
        // Set initial camera scale based on the sizes of the bodies and the window size
        float maxRadius = 0.0f;

        float radius = m_Mars->GetRadius();
        if (radius > maxRadius)
            maxRadius = radius;

        uint32_t minDimension = std::min<float>(m_Width, m_Height);
        m_Scaling = minDimension / maxRadius / 2.0f;
    }

}
