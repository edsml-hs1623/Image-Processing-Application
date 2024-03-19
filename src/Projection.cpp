#include "Projection.h"
#include "stb_image_write.h"
#include <vector>
#include <numeric> // For std::accumulate

void Projection::mip(const Volume& volume, const std::string& outputPath) {
    int width = volume.getWidth();
    int height = volume.getHeight();
    int channels = volume.getChannels();
    std::vector<unsigned char> projectionData(width * height * channels, 0);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            unsigned char maxIntensity = 0;
            for (int z = 0; z < volume.getDepth(); ++z) {
                unsigned char intensity = volume.getData()[z][(y * width + x) * channels];
                if (intensity > maxIntensity) {
                    maxIntensity = intensity;
                }
            }
            projectionData[y * width + x] = maxIntensity;
        }
    }

    stbi_write_png(outputPath.c_str(), width, height, channels, projectionData.data(), width * channels);
}

void Projection::minip(const Volume& volume, const std::string& outputPath) {
    int width = volume.getWidth();
    int height = volume.getHeight();
    int channels = volume.getChannels();
    std::vector<unsigned char> projectionData(width * height * channels, std::numeric_limits<unsigned char>::max());

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            unsigned char minIntensity = std::numeric_limits<unsigned char>::max();
            for (int z = 0; z < volume.getDepth(); ++z) {
                unsigned char intensity = volume.getData()[z][(y * width + x) * channels];
                if (intensity < minIntensity) {
                    minIntensity = intensity;
                }
            }
            projectionData[y * width + x] = minIntensity;
        }
    }

    stbi_write_png(outputPath.c_str(), width, height, channels, projectionData.data(), width * channels);
}

void Projection::aip(const Volume& volume, const std::string& outputPath) {
    int width = volume.getWidth();
    int height = volume.getHeight();
    int channels = volume.getChannels();
    std::vector<unsigned char> projectionData(width * height * channels, 0);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            unsigned long long totalIntensity = 0;
            for (int z = 0; z < volume.getDepth(); ++z) {
                totalIntensity += volume.getData()[z][(y * width + x) * channels];
            }
            projectionData[y * width + x] = static_cast<unsigned char>(totalIntensity / volume.getDepth());
        }
    }

    stbi_write_png(outputPath.c_str(), width, height, channels, projectionData.data(), width * channels);
}
