#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace Visualization
{
    class ImageResizer
    {
    public:
        ImageResizer() = default;
        ~ImageResizer() = default;

        static void resizeImage(const std::string &inputFilePath, int newWidth, int newHeight, std::vector<uint32_t> &outputData);
    };

}