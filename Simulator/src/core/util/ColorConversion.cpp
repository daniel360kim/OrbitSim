/**
 * @file ColorConversion.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief
 * @version 0.1
 * @date 2023-07-21
 *
 *
 */

#include "ColorConversion.h"

namespace Visualization
{
    uint32_t ColorConversion::colorToInt(glm::vec4 color)
    {
        uint32_t colorInt = (static_cast<uint32_t>(color.a * 255) << 24) |
                            (static_cast<uint32_t>(color.b * 255) << 16) |
                            (static_cast<uint32_t>(color.g * 255) << 8) |
                            (static_cast<uint32_t>(color.r * 255) << 0);
        return colorInt;
    }

    glm::vec4 ColorConversion::intToColor(uint32_t color)
    {
        glm::vec4 colorVec;
        colorVec.a = static_cast<float>((color >> 24) & 0xFF) / 255.0f;
        colorVec.b = static_cast<float>((color >> 16) & 0xFF) / 255.0f;
        colorVec.g = static_cast<float>((color >> 8) & 0xFF) / 255.0f;
        colorVec.r = static_cast<float>((color >> 0) & 0xFF) / 255.0f;
        return colorVec;

    }

}