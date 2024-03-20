#include "Projection.h"
#include "stb_image_write.h"
#include <vector>
#include <numeric> // For std::accumulate
#include <limits>
#include <algorithm>

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
