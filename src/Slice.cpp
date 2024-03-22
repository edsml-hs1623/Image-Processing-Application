#include "Slice.h"
#include "stb_image_write.h"
#include <vector>
#include <iostream>
#include <cassert> // For assert to validate input

void Slice::sliceXZ(const Volume& volume, int y, const std::string& outputPath) {
    assert(y > 0 && y <= volume.getHeight()); // Ensure y is within bounds
    int width = volume.getWidth();
    int depth = volume.getDepth();
    int channels = volume.getChannels();
    std::vector<unsigned char> sliceData(width * depth * channels);

    // Adjust y to 0-based index for internal use
    y = y - 1;

    const auto& allData = volume.getData(); // Correctly access all volume data once

    for (int z = 0; z < depth; ++z) {
        const auto& currentSliceData = allData[z]; // Access specific slice
        for (int x = 0; x < width; ++x) {
            for (int ch = 0; ch < channels; ++ch) {
                int index = ((y * width) + x) * channels + ch;
                sliceData[(z * width + x) * channels + ch] = currentSliceData[index]; // Corrected access
            }
        }
    }

    stbi_write_png(outputPath.c_str(), width, depth, channels, sliceData.data(), width * channels);
}

void Slice::sliceYZ(const Volume& volume, int x, const std::string& outputPath) {
    assert(x > 0 && x <= volume.getWidth()); // Ensure x is within bounds
    int height = volume.getHeight();
    int depth = volume.getDepth();
    int channels = volume.getChannels();
    std::vector<unsigned char> sliceData(height * depth * channels);

    // Adjust x to 0-based index for internal use
    x = x - 1;

    const auto& allData = volume.getData(); // Correctly access all volume data once

    for (int z = 0; z < depth; ++z) {
        const auto& currentSliceData = allData[z]; // Access specific slice
        for (int y = 0; y < height; ++y) {
            for (int ch = 0; ch < channels; ++ch) {
                int index = (y * volume.getWidth() + x) * channels + ch;
                sliceData[(z * height + y) * channels + ch] = currentSliceData[index]; // Corrected access
            }
        }
    }

    stbi_write_png(outputPath.c_str(), height, depth, channels, sliceData.data(), height * channels);
}
