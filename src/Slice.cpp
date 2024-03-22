/**
 * @file Slice.cpp
 *
 * @brief Implements methods for extracting 2D slices from a 3D volume.
 *
 * This file provides the implementation for two key functions of the Slice class,
 * allowing for the extraction of 2D slices from a specified 3D volume along different planes.
 * The extracted slices can be used for analysis, visualization, or further processing.
 * The functions are capable of generating slices along the XZ and YZ planes,
 * given a specific index along the Y and X axes, respectively.
 *
 * The output slices are saved as PNG files to a specified path. This implementation
 * relies on the stb_image_write library to handle the image writing process.
 *
 * Dependencies:
 *   - Slice.h for the declaration of the Slice class.
 *   - stb_image_write.h for writing the slice images as PNG files.
 *   - Volume.h for accessing the volume data.
 *   - Standard libraries: <vector>, <iostream>, and <cassert>.
 * @author acse-yw3523,edsml-lwk16, acse-ad2123, 
 *         edsml-hs1623, acse-xg1123, edsml-st2923,
 *         Group: selection sort.
 */
#include "Slice.h"
#include "stb_image_write.h"
#include <vector>
#include <iostream>
#include <cassert> // For assert to validate input

/**
 * @brief Extracts and saves a slice from the given volume along the XZ plane at a specified Y index.
 *
 * @param volume The 3D volume from which to extract the slice.
 * @param y The index along the Y-axis at which to extract the slice. Must be within the bounds of the volume's height.
 * @param outputPath The filesystem path where the resulting slice image will be saved as a PNG file.
 *
 * @note The function asserts that the given Y index is within the volume's bounds.
 */
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

/**
 * @brief Extracts and saves a slice from the given volume along the YZ plane at a specified X index.
 *
 * @param volume The 3D volume from which to extract the slice.
 * @param x The index along the X-axis at which to extract the slice. Must be within the bounds of the volume's width.
 * @param outputPath The filesystem path where the resulting slice image will be saved as a PNG file.
 *
 * @note The function asserts that the given X index is within the volume's bounds.
 */
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
