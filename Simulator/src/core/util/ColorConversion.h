/**
 * @file ColorConversion.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief 
 * @version 0.1
 * @date 2023-07-21
 * 
 * 
 */

#pragma once

#include <glm/glm.hpp>
#include <cstdint>

namespace Visualization
{
    class ColorConversion
    {
    public:
        static uint32_t colorToInt(glm::vec4 color);
        static glm::vec4 intToColor(uint32_t color);
    };
} // namespace Visualization