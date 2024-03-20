#include "VolumeFilter.h"
#include <cmath>
#include <algorithm>
#include <numeric>
#include <vector>
#include <iostream>

Volume VolumeFilter::applyGaussianFilter(const Volume& originalVolume, int kernelSize, double sigma) {
    auto kernel = generateGaussianKernel(kernelSize, sigma);
    int halfSize = kernelSize / 2;

    Volume filteredVolume = originalVolume; // Make a copy to preserve the original volume

    // Adjust the iteration limits to avoid the need for boundary checking
    for (int z = halfSize; z < originalVolume.getDepth() - halfSize; ++z) {

        for (int y = halfSize; y < originalVolume.getHeight() - halfSize; ++y) {
            for (int x = halfSize; x < originalVolume.getWidth() - halfSize; ++x) {

                double filteredValue = 0.0;
                double kernelSum = 0.0;

                for (int kz = -halfSize; kz <= halfSize; ++kz) {
                    int zz = z + kz;

                    for (int ky = -halfSize; ky <= halfSize; ++ky) {
                        int yy = y + ky;

                        for (int kx = -halfSize; kx <= halfSize; ++kx) {
                            int xx = x + kx;

                            auto voxelValue = static_cast<double>(originalVolume.getData(zz)[yy * originalVolume.getWidth() + xx]);
                            double weight = kernel[kz + halfSize][ky + halfSize][kx + halfSize];

                            filteredValue += voxelValue * weight;
                            kernelSum += weight;
                        }
                    }
                }

                // Apply the filter
                if (kernelSum > 0) {
                    unsigned char newValue = static_cast<unsigned char>(std::round(filteredValue / kernelSum));
                    filteredVolume.setVoxelValue(z, y, x, newValue);
                }
            }
        }
    }

    return filteredVolume;
}

Volume VolumeFilter::applyMedianFilter(const Volume& volume, int kernelSize) {
    // Placeholder for the Median filter application logic
    // You will need to fill in this method with the actual filtering logic
    Volume filteredVolume = volume; // This line is just a placeholder
    return filteredVolume;
}

std::vector<std::vector<std::vector<double>>> VolumeFilter::generateGaussianKernel(int kernelSize, double sigma) {
    int halfSize = kernelSize / 2;
    std::vector<std::vector<std::vector<double>>> kernel(kernelSize,
                                                         std::vector<std::vector<double>>(kernelSize,
                                                                                          std::vector<double>(kernelSize)));
    double sum = 0.0;

    for (int x = -halfSize; x <= halfSize; x++) {
        for (int y = -halfSize; y <= halfSize; y++) {
            for (int z = -halfSize; z <= halfSize; z++) {
                double distance = x * x + y * y + z * z;
                double value = std::exp(-distance / (2 * sigma * sigma)) / (std::sqrt(2 * M_PI) * sigma);
                kernel[x + halfSize][y + halfSize][z + halfSize] = value;
                sum += value;
            }
        }
    }

    // Normalize the kernel
    for (auto &slice : kernel) {
        for (auto &row : slice) {
            for (double &val : row) {
                val /= sum;
            }
        }
    }

    return kernel;
}

unsigned char VolumeFilter::calculateMedianValue(std::vector<unsigned char>& values) {
    std::nth_element(values.begin(), values.begin() + values.size() / 2, values.end());
    return values[values.size() / 2];
}