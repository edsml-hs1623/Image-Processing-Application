/**
 * @file ThreeDFilter.h
 *
 * @brief Implementation of 3D image processing techniques, including Gaussian and Median blurs.
 *
 * This file provides the implementation of various 3D image processing techniques, primarily focusing on
 * Gaussian and Median blur filters. These filters are applied to Volume objects representing 3D images or data sets.
 * The Gaussian blur is implemented by convolving the image with a Gaussian kernel, while the Median blur is
 * realized through a non-linear filtering approach that replaces each voxel's intensity with the median
 * of intensities in its neighborhood.
 *
 * Key Functionalities:
 *   - Gaussian Blur: Applies a Gaussian blur filter to smooth 3D images, useful for reducing image noise and details.
 *   - Median Blur: Applies a Median blur filter to reduce noise without creating artifacts.
 *   - Utility Functions: Includes functions for value clamping and kernel generation.
 *
 * Usage:
 *   Volume myVolume;
 *   ThreeDFilter filter;
 *   filter.gaussianBlur(myVolume, 5, 1.0); // Apply Gaussian blur with a 5x5x5 kernel and sigma = 1.0
 *   filter.medianBlur(myVolume, 3); // Apply Median blur with a 3x3x3 kernel
 *
 * @note The filters modify the Volume object in-place. Ensure that you have a backup of the original
 *       data if needed.
 *
 * Dependencies:
 *   - Volume.h for the Volume class definition and manipulation.
 *   - <cmath> and <vector> for mathematical operations and data storage.
 *   - <numeric> and <array> for additional array operations and fixed-size array storage.
 * @author acse-yw3523,edsml-lwk16, acse-ad2123, 
 *         edsml-hs1623, acse-xg1123, edsml-st2923,
 *         Group: selection sort.
 */
#include "ThreeDFilter.h"
#include <cmath>
#include <vector>
#include <numeric>
#include <array>

template <typename T>
T clamp(T val, T minVal, T maxVal) {
    if (val < minVal) return minVal;
    if (val > maxVal) return maxVal;
    return val;
}

/**
 * @brief Calculates the Gaussian value for a given point and sigma.
 * @param x The x-coordinate of the point.
 * @param y The y-coordinate of the point.
 * @param z The z-coordinate of the point.
 * @param sigma The standard deviation of the Gaussian distribution.
 * @return The Gaussian function value at the given point.
 */
float ThreeDFilter::gaussian(float x, float y, float z, float sigma) {
    return std::exp(-(x * x + y * y + z * z) / (2 * sigma * sigma)) / (std::sqrt(2 * M_PI) * sigma);
}

/**
 * @brief Applies a Gaussian blur to a given volume.
 * @param volume A reference to the Volume object to blur.
 * @param kernelSize The size of the Gaussian kernel (must be an odd number).
 * @param sigma The standard deviation of the Gaussian distribution used for the kernel.
 */
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

//Optimized version of Gaussian Blur
// std::vector<float> ThreeDFilter::precomputeGaussianKernel(int kernelSize, float sigma) {
//     std::vector<float> kernel(kernelSize);
//     int halfSize = kernelSize / 2;
//     float sum = 0.0f; // For normalization

//     for (int i = -halfSize; i <= halfSize; ++i) {
//         float exponent = std::exp(-0.5f * i * i / (sigma * sigma));
//         kernel[i + halfSize] = exponent / (std::sqrt(2 * M_PI) * sigma);
//         sum += kernel[i + halfSize];
//     }

//     // Normalize the kernel to ensure the sum is 1.
//     for (auto &value : kernel) {
//         value /= sum;
//     }

//     return kernel;
// }

// void ThreeDFilter::applyGaussianBlur1D(std::vector<unsigned char>& line, const std::vector<float>& kernel, int halfSize) {
//     std::vector<unsigned char> tempLine(line.size());

//     // Apply the 1D blur for a line in one dimension
//     for (size_t i = 0; i < line.size(); ++i) {
//         float sum = 0.0;
//         // Apply the kernel to each pixel in the line
//         for (int k = -halfSize; k <= halfSize; ++k) {
//             int index = std::clamp(static_cast<int>(i) + k, 0, static_cast<int>(line.size()) - 1);
//             sum += line[index] * kernel[halfSize + k];
//         }
//         tempLine[i] = static_cast<unsigned char>(std::clamp(static_cast<int>(sum), 0, 255));
//     }

//     line = std::move(tempLine);
// }

// void ThreeDFilter::gaussianBlur(Volume& volume, int kernelSize, float sigma) {
//     // Assuming sigma is constant, precompute the kernel
//     auto kernel = precomputeGaussianKernel(sigma, kernelSize);
//     int halfSize = kernelSize / 2;

//     // Get a copy of the data from the volume since getData() returns a const reference
//     auto data = volume.getData();
//     int width = volume.getWidth();
//     int height = volume.getHeight();
//     int depth = volume.getDepth();
//     int channels = volume.getChannels();

//     // Temporary storage for the intermediate results after blurring in each direction
//     std::vector<std::vector<unsigned char>> tempDataX(depth, std::vector<unsigned char>(width * height * channels));
//     std::vector<std::vector<unsigned char>> tempDataY(depth, std::vector<unsigned char>(width * height * channels));
//     std::vector<std::vector<unsigned char>> tempDataZ(depth, std::vector<unsigned char>(width * height * channels));

//     // Apply the Gaussian blur in the x-direction
//     for (int z = 0; z < depth; ++z) {
//         for (int y = 0; y < height; ++y) {
//             for (int ch = 0; ch < channels; ++ch) {
//                 std::vector<unsigned char> line(width);
//                 for (int x = 0; x < width; ++x) {
//                     line[x] = data[z][(y * width + x) * channels + ch];
//                 }
//                 applyGaussianBlur1D(line, kernel, halfSize);
//                 for (int x = 0; x < width; ++x) {
//                     tempDataX[z][(y * width + x) * channels + ch] = line[x];
//                 }
//             }
//         }
//     }

//     // Apply the Gaussian blur in the y-direction
//     for (int z = 0; z < depth; ++z) {
//         for (int x = 0; x < width; ++x) {
//             for (int ch = 0; ch < channels; ++ch) {
//                 std::vector<unsigned char> line(height);
//                 for (int y = 0; y < height; ++y) {
//                     line[y] = tempDataX[z][(y * width + x) * channels + ch];
//                 }
//                 applyGaussianBlur1D(line, kernel, halfSize);
//                 for (int y = 0; y < height; ++y) {
//                     tempDataY[z][(y * width + x) * channels + ch] = line[y];
//                 }
//             }
//         }
//     }

//     // Apply the Gaussian blur in the z-direction
//     for (int y = 0; y < height; ++y) {
//         for (int x = 0; x < width; ++x) {
//             for (int ch = 0; ch < channels; ++ch) {
//                 std::vector<unsigned char> line(depth);
//                 for (int z = 0; z < depth; ++z) {
//                     line[z] = tempDataY[z][(y * width + x) * channels + ch];
//                 }
//                 applyGaussianBlur1D(line, kernel, halfSize);
//                 for (int z = 0; z < depth; ++z) {
//                     tempDataZ[z][(y * width + x) * channels + ch] = line[z];
//                 }
//             }
//         }
//     }

//     // Once all the blurring is done, use setData to update the volume's data
//     volume.setData(tempDataZ);
// }



// void ThreeDFilter::medianBlur(Volume& volume, int kernelSize) {
//     int width = volume.getWidth();
//     int height = volume.getHeight();
//     int depth = volume.getDepth();
//     int channels = volume.getChannels();
//     int halfSize = kernelSize / 2;

//     std::vector<std::vector<unsigned char>> newVolumeData(depth, std::vector<unsigned char>(width * height * channels));

//     for (int z = 0; z < depth; z++) {
//         for (int y = 0; y < height; y++) {
//             for (int x = 0; x < width; x++) {
//                 for (int ch = 0; ch < channels; ch++) {
//                     std::array<int, 256> histogram = {0};
//                     for (int kz = -halfSize; kz <= halfSize; kz++) {
//                         for (int ky = -halfSize; ky <= halfSize; ky++) {
//                             for (int kx = -halfSize; kx <= halfSize; kx++) {
//                                 int nx = std::max(0, std::min(x + kx, width - 1));
//                                 int ny = std::max(0, std::min(y + ky, height - 1));
//                                 int nz = std::max(0, std::min(z + kz, depth - 1));
//                                 unsigned char value = volume.getData()[nz][((ny * width + nx) * channels) + ch];
//                                 histogram[value]++;
//                             }
//                         }
//                     }

//                     int count = 0;
//                     unsigned char medianValue = 0;
//                     for (int i = 0; i < 256; i++) {
//                         count += histogram[i];
//                         if (count >= (kernelSize * kernelSize * kernelSize) / 2) {
//                             medianValue = i;
//                             break;
//                         }
//                     }

//                     newVolumeData[z][((y * width + x) * channels) + ch] = medianValue;
//                 }
//             }
//         }
//     }

//     volume.setData(newVolumeData);
// }

/**
 * @brief Applies a median blur to a given volume.
 * @param volume A reference to the Volume object to apply the median blur.
 * @param kernelSize The size of the cubic kernel (must be an odd number).
 */
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
                    unsigned char minVal = 255;
                    unsigned char maxVal = 0;
                    for (int kz = -halfSize; kz <= halfSize; kz++) {
                        for (int ky = -halfSize; ky <= halfSize; ky++) {
                            for (int kx = -halfSize; kx <= halfSize; kx++) {
                                int nx = clamp(x + kx, 0, width - 1);
                                int ny = clamp(y + ky, 0, height - 1);
                                int nz = clamp(z + kz, 0, depth - 1);
                                unsigned char value = volume.getData()[nz][((ny * width + nx) * channels) + ch];
                                maxVal = std::max(maxVal, value);
                                minVal = std::min(minVal, value);
                            }
                        }
                    }

                    // Approximating median based on uniform distribution assumption
                    unsigned char approxMedian = (minVal + maxVal) / 2;
                    newVolumeData[z][((y * width + x) * channels) + ch] = approxMedian;
                }
            }
        }
    }

    volume.setData(newVolumeData);
}

/**
 * @brief Calculates the median value from a vector of unsigned characters.
 * @param values A reference to the vector of unsigned char values.
 * @return The median value as an unsigned char.
 */
unsigned char ThreeDFilter::median(std::vector<unsigned char>& values) {
    selectionSort(values);
    if (values.size() % 2 == 0) {
        return (values[values.size() / 2 - 1] + values[values.size() / 2]) / 2;
    } else {
        return values[values.size() / 2];
    }
}

/**
 * @brief Sorts a vector of unsigned char values using the selection sort algorithm.
 * @param arr A reference to the vector of unsigned char values to be sorted.
 */
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
