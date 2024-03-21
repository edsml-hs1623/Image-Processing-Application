#include "ThreeDFilter.h"
#include <cmath>
#include <vector>
#include <numeric>
#include <array>

float ThreeDFilter::gaussian(float x, float y, float z, float sigma) {
    return std::exp(-(x * x + y * y + z * z) / (2 * sigma * sigma)) / (std::sqrt(2 * M_PI) * sigma);
}

void ThreeDFilter::gaussianBlur(Volume& volume, int kernelSize, float sigma) {
    int width = volume.getWidth();
    int height = volume.getHeight();
    int depth = volume.getDepth();
    int channels = volume.getChannels();

    // Preparing the Gaussian kernel
    int halfSize = kernelSize / 2;
    std::vector<float> kernel(kernelSize * kernelSize * kernelSize);
    float kernelSum = 0;
    for (int x = -halfSize; x <= halfSize; x++) {
        for (int y = -halfSize; y <= halfSize; y++) {
            for (int z = -halfSize; z <= halfSize; z++) {
                float value = gaussian(x, y, z, sigma);
                kernel[(x + halfSize) * kernelSize * kernelSize + (y + halfSize) * kernelSize + (z + halfSize)] = value;
                kernelSum += value;
            }
        }
    }

    // Normalize the kernel
    for (auto& value : kernel) {
        value /= kernelSum;
    }

    std::vector<std::vector<unsigned char>> newVolumeData(depth, std::vector<unsigned char>(width * height * channels));

    // Apply Gaussian Blur
    for (int z = 0; z < depth; z++) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                for (int ch = 0; ch < channels; ch++) {
                    float blurredPixel = 0;
                    for (int kx = -halfSize; kx <= halfSize; kx++) {
                        for (int ky = -halfSize; ky <= halfSize; ky++) {
                            for (int kz = -halfSize; kz <= halfSize; kz++) {
                                int xx = std::min(std::max(x + kx, 0), width - 1);
                                int yy = std::min(std::max(y + ky, 0), height - 1);
                                int zz = std::min(std::max(z + kz, 0), depth - 1);
                                blurredPixel += volume.getData()[zz][((yy * width + xx) * channels) + ch] * kernel[(kx + halfSize) * kernelSize * kernelSize + (ky + halfSize) * kernelSize + (kz + halfSize)];
                            }
                        }
                    }
                    newVolumeData[z][((y * width + x) * channels) + ch] = std::min(std::max(int(blurredPixel), 0), 255);
                }
            }
        }
    }

    volume.setData(newVolumeData);
}

void ThreeDFilter::medianBlur(Volume& volume, int kernelSize) {
    int width = volume.getWidth();
    int height = volume.getHeight();
    int depth = volume.getDepth();
    int channels = volume.getChannels();
    int halfSize = kernelSize / 2;

    std::vector<std::vector<unsigned char>> newVolumeData(depth, std::vector<unsigned char>(width * height * channels));

    for (int z = 0; z < depth; z++) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                for (int ch = 0; ch < channels; ch++) {
                    std::array<int, 256> histogram = {0};
                    for (int kz = -halfSize; kz <= halfSize; kz++) {
                        for (int ky = -halfSize; ky <= halfSize; ky++) {
                            for (int kx = -halfSize; kx <= halfSize; kx++) {
                                int nx = std::max(0, std::min(x + kx, width - 1));
                                int ny = std::max(0, std::min(y + ky, height - 1));
                                int nz = std::max(0, std::min(z + kz, depth - 1));
                                unsigned char value = volume.getData()[nz][((ny * width + nx) * channels) + ch];
                                histogram[value]++;
                            }
                        }
                    }

                    int count = 0;
                    unsigned char medianValue = 0;
                    for (int i = 0; i < 256; i++) {
                        count += histogram[i];
                        if (count >= (kernelSize * kernelSize * kernelSize) / 2) {
                            medianValue = i;
                            break;
                        }
                    }

                    newVolumeData[z][((y * width + x) * channels) + ch] = medianValue;
                }
            }
        }
    }

    volume.setData(newVolumeData);
}

unsigned char ThreeDFilter::median(std::vector<unsigned char>& values) {
    selectionSort(values);
    if (values.size() % 2 == 0) {
        return (values[values.size() / 2 - 1] + values[values.size() / 2]) / 2;
    } else {
        return values[values.size() / 2];
    }
}

void ThreeDFilter::selectionSort(std::vector<unsigned char>& arr) {
    for (size_t i = 0; i < arr.size() - 1; i++) {
        size_t min_idx = i;
        for (size_t j = i + 1; j < arr.size(); j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        // Swap the found minimum element with the first element
        unsigned char temp = arr[min_idx];
        arr[min_idx] = arr[i];
        arr[i] = temp;
    }
}