/**
 * @file ImageResizer.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief
 * @version 0.1
 * @date 2023-07-15
 *
 *
 */

#include "ImageResizer.h"

#include <stb_image.h>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize.h>


namespace Visualization
{
    void ImageResizer::resizeImage(const std::string &inputFilePath, int newWidth, int newHeight, std::vector<uint32_t> &outputData)
    {
        int originalWidth, originalHeight, originalChannels;
        stbi_uc *inputData = stbi_load(inputFilePath.c_str(), &originalWidth, &originalHeight, &originalChannels, STBI_rgb_alpha);
        if (!inputData)
        {
            outputData.clear(); // Failed to load the image
            return;
        }

        outputData.resize(newWidth * newHeight);

        // Resize the image using stb_image_resize function
        stbir_resize_uint8(inputData, originalWidth, originalHeight, 0, reinterpret_cast<stbi_uc *>(outputData.data()), newWidth, newHeight, 0, STBI_rgb_alpha);

        stbi_image_free(inputData);
    }
}