/**
 * @file Projection.cpp
 *
 * @brief Implements methods for generating 2D projections from 3D volumes.
 *
 * This file provides the implementation for several projection techniques to visualize 3D volume data in 2D.
 * It supports Maximum Intensity Projection (MIP), Minimum Intensity Projection (MinIP), and Average Intensity
 * Projection (AIP). These methods project the highest, lowest, and average intensity values, respectively, across
 * a specified range of slices from the volume onto a 2D plane.
 *
 * The output projections are saved as PNG files using the stb_image_write library. This allows for easy visualization
 * and analysis of the volume data.
 *
 * Dependencies:
 *   - Projection.h for the declaration of the Projection class.
 *   - stb_image_write.h for writing the projection images as PNG files.
 *   - Volume.h for accessing the volume data.
 *   - Standard libraries: <vector>, <numeric>, <limits>, and <algorithm>.
 * @author acse-yw3523,edsml-lwk16, acse-ad2123, 
 *         edsml-hs1623, acse-xg1123, edsml-st2923,
 *         Group: selection sort.
 */
#include "Projection.h"
#include "stb_image_write.h"
#include <vector>
#include <numeric> // For std::accumulate
#include <limits>
#include <algorithm>

/**
 * @brief Generates a Maximum Intensity Projection (MIP) from a given volume.
 *
 * This method projects the maximum intensity value encountered along the Z-axis for each pixel
 * in the X-Y plane, within the specified range of slices. The result is a 2D image representing
 * the highest intensity values across the specified Z range.
 *
 * @param volume The 3D volume from which to generate the MIP.
 * @param outputPath The filesystem path where the resulting MIP image will be saved as a PNG file.
 * @param minZ The starting slice index for the projection range.
 * @param maxZ The ending slice index for the projection range.
 */
void Projection::mip(const Volume& volume, const std::string& outputPath, int minZ, int maxZ) {
    int width = volume.getWidth();
    int height = volume.getHeight();
    int channels = volume.getChannels();
    std::vector<unsigned char> projectionData(width * height * channels, 0);

    const auto& allData = volume.getData(); // Correctly access all volume data once

    // Adjust for 1-based indexing and validate range
    minZ = std::max(minZ - 1, 0); // Ensure not below 0
    maxZ = std::min(maxZ - 1, static_cast<int>(allData.size()) - 1); // Ensure not beyond the last index

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            unsigned char maxIntensity = 0;
            for (int z = minZ; z <= maxZ; ++z) {
                const auto& sliceData = allData[z]; // Access specific slice
                unsigned char intensity = sliceData[(y * width + x) * channels];
                if (intensity > maxIntensity) {
                    maxIntensity = intensity;
                }
            }
            projectionData[(y * width + x) * channels] = maxIntensity;
        }
    }

    stbi_write_png(outputPath.c_str(), width, height, channels, projectionData.data(), width * channels);
}

/**
 * @brief Generates a Minimum Intensity Projection (MinIP) from a given volume.
 *
 * Similar to MIP but projects the minimum intensity value encountered along the Z-axis for each
 * pixel in the X-Y plane, within a specified range of slices. The result is a 2D image showcasing
 * the lowest intensity values across the specified Z range.
 *
 * @param volume The 3D volume from which to generate the MinIP.
 * @param outputPath The filesystem path where the resulting MinIP image will be saved as a PNG file.
 * @param minZ The starting slice index for the projection range.
 * @param maxZ The ending slice index for the projection range.
 */
void Projection::minip(const Volume& volume, const std::string& outputPath, int minZ, int maxZ) {
    int width = volume.getWidth();
    int height = volume.getHeight();
    int channels = volume.getChannels();
    std::vector<unsigned char> projectionData(width * height * channels, std::numeric_limits<unsigned char>::max());

    const auto& allData = volume.getData(); // Correctly access all volume data once

    // Adjust for 1-based indexing and validate range
    minZ = std::max(minZ - 1, 0);
    maxZ = std::min(maxZ - 1, volume.getDepth() - 1);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            unsigned char minIntensity = std::numeric_limits<unsigned char>::max();
            for (int z = minZ; z <= maxZ; ++z) {
                const auto& sliceData = allData[z]; // Get slice data for z
                unsigned char intensity = sliceData[(y * width + x) * channels];
                if (intensity < minIntensity) {
                    minIntensity = intensity;
                }
            }
            projectionData[(y * width + x) * channels] = minIntensity; // Correct indexing for multi-channel support
        }
    }

    stbi_write_png(outputPath.c_str(), width, height, channels, projectionData.data(), width * channels);
}

/**
 * @brief Generates an Average Intensity Projection (AIP) from a given volume.
 *
 * Computes the average intensity value for each pixel in the X-Y plane across the specified Z
 * range of slices. The result is a 2D image that represents the average intensity values within
 * the specified slice range, providing a comprehensive overview of the volume's density.
 *
 * @param volume The 3D volume from which to generate the AIP.
 * @param outputPath The filesystem path where the resulting AIP image will be saved as a PNG file.
 * @param minZ The starting slice index for the projection range.
 * @param maxZ The ending slice index for the projection range.
 */
void Projection::aip(const Volume& volume, const std::string& outputPath, int minZ, int maxZ) {
    int width = volume.getWidth();
    int height = volume.getHeight();
    int channels = volume.getChannels();
    std::vector<unsigned char> projectionData(width * height * channels, 0);

    const auto& allData = volume.getData(); // Correctly access all volume data once



    // Adjust for 1-based indexing and validate range
    minZ = std::max(minZ - 1, 0);
    maxZ = std::min(maxZ - 1, volume.getDepth() - 1);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            unsigned long long totalIntensity = 0;
            for (int z = minZ; z <= maxZ; ++z) {
                const auto& sliceData = allData[z]; // Get slice data for z
                totalIntensity += sliceData[(y * width + x) * channels];
            }
            projectionData[(y * width + x) * channels] = static_cast<unsigned char>(totalIntensity / (maxZ - minZ + 1)); // Correct indexing for multi-channel support
        }
    }

    stbi_write_png(outputPath.c_str(), width, height, channels, projectionData.data(), width * channels);
}
