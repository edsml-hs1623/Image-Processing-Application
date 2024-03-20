/**
 * @file ImageBlur.cpp
 *
 * @brief Implementation of the ImageBlur class for blurring images.
 *
 * This file contains the implementation of the ImageBlur class, which offers functionalities
 * for applying various types of blurs to images. Supported blur types include Median Blur, Box Blur,
 * and Gaussian Blur. Each blur type is implemented as a separate method. The class extends the Filter class,
 * allowing for seamless integration into image processing pipelines.
 *
 * Key Features:
 *   - Provides Median, Box, and Gaussian blurring techniques.
 *   - Extensible to include other blur types.
 *   - Seamless integration with the Image class for easy application to images.
 *
 * Usage:
 *   Image img; // An instance of the Image class
 *   ImageBlur gaussianBlur(ImageBlur::Gaussian, 5); // Gaussian blur with kernel size 5
 *   gaussianBlur.apply(img); // Apply Gaussian blur to 'img'
 *
 * @note This class depends on the Image class for handling image data. Ensure that the Image class
 *       is compatible and included in your project.
 */

#include "ImageBlur.h"
#include <algorithm>
#include <cmath>
#include <numeric>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


/**
* Constructor: Constructor for the ImageBlur class.
* Initializes an ImageBlur object with a specified blur type and kernel size.
*
* @param type The type of blur to apply (e.g., Gaussian, Median, Box).
* @param kernelSize The size of the kernel to use for blurring.
*/
ImageBlur::ImageBlur(BlurType type, int kernelSize) : blurType(type), kernelSize(kernelSize) {}

/**
 * Destructor: Destructor for the ImageBlur class.
 */
ImageBlur::~ImageBlur() {}

/**
 * Applies the selected blur type to an image.
 *
 * @param image The image to be blurred.
 */
void ImageBlur::apply(Image &image) {
    switch(blurType) {
        case Median:
            applyMedianBlur(image);
            break;
        case Box:
            applyBoxBlur(image);
            break;
        case Gaussian:
            applyGaussianBlur(image);
            break;
        default:
            std::cerr << "Unsupported blur type" << std::endl;
    }
}

/**
 * Applies box blur to an image using a defined kernel size.
 * This method averages the pixels within the kernel area.
 *
 * @param image The image to apply the box blur on.
 */
void ImageBlur::applyBoxBlur(Image &image) {
    int width = image.getWidth();
    int height = image.getHeight();
    int channels = image.getChannels();
    unsigned char* data = image.getData();
    std::vector<unsigned char> newData(width * height * channels);

    int halfKernel = kernelSize / 2;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            for (int c = 0; c < channels; ++c) {
                int sum = 0;
                int count = 0;
                for (int ky = -halfKernel; ky <= halfKernel; ++ky) {
                    for (int kx = -halfKernel; kx <= halfKernel; ++kx) {
                        int nx = x + kx;
                        int ny = y + ky;
                        if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                            int index = (ny * width + nx) * channels + c;
                            sum += data[index];
                            count++;
                        }
                    }
                }
                newData[y * width * channels + x * channels + c] = sum / count;
            }
        }
    }

    std::copy(newData.begin(), newData.end(), data);
}

/**
 * Finds the median value in a vector of unsigned char values.
 *
 * @param values The vector containing the values to find the median of.
 * @return The median value as an unsigned char.
 */
unsigned char ImageBlur::findMedian(std::vector<unsigned char>& values) {
    selectionSort(values);
    if (values.size() % 2 == 0) {
        return (values[values.size() / 2 - 1] + values[values.size() / 2]) / 2;
    } else {
        return values[values.size() / 2];
    }
}

/**
 * Sorts a vector of unsigned char values using the selection sort algorithm.
 *
 * @param arr The vector to be sorted.
 */
void ImageBlur::selectionSort(std::vector<unsigned char>& arr) {
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

/**
 * Applies median blur to an image. This method replaces each pixel's value with the median
 * value of the intensities in the kernel area around the pixel.
 *
 * @param image The image to apply the median blur on.
 */
void ImageBlur::applyMedianBlur(Image &image) {
    int width = image.getWidth();
    int height = image.getHeight();
    int channels = image.getChannels();
    unsigned char* data = image.getData();
    std::vector<unsigned char> newData(width * height * channels);

    auto getMedian = [](std::vector<unsigned char>& values) -> unsigned char {
        std::nth_element(values.begin(), values.begin() + values.size() / 2, values.end());
        return values[values.size() / 2];
    };

    int halfKernel = kernelSize / 2;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            for (int c = 0; c < channels; ++c) {
                std::vector<unsigned char> kernelValues;
                for (int ky = -halfKernel; ky <= halfKernel; ++ky) {
                    for (int kx = -halfKernel; kx <= halfKernel; ++kx) {
                        int nx = x + kx;
                        int ny = y + ky;
                        if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                            int index = (ny * width + nx) * channels + c;
                            kernelValues.push_back(data[index]);
                        }
                    }
                }
                newData[y * width * channels + x * channels + c] = getMedian(kernelValues);
            }
        }
    }

    std::copy(newData.begin(), newData.end(), data);
}

/**
 * Applies Gaussian blur to an image using a Gaussian kernel.
 * The standard deviation (sigma) of the Gaussian distribution is calculated,
 * and the kernel is applied to blur the image.
 *
 * @param image The image to apply the Gaussian blur on.
 */
void ImageBlur::applyGaussianBlur(Image &image) {
    double sigma = 1.0; // Standard deviation, adjust as needed
    int width = image.getWidth();
    int height = image.getHeight();
    int channels = image.getChannels();
    unsigned char* data = image.getData();
    std::vector<unsigned char> newData(width * height * channels);

    // Precompute the Gaussian kernel
    int halfKernel = kernelSize / 2;
    std::vector<double> kernel(kernelSize * kernelSize);
    double sum = 0.0;
    for (int i = -halfKernel; i <= halfKernel; ++i) {
        for (int j = -halfKernel; j <= halfKernel; ++j) {
            double weight = std::exp(-(i * i + j * j) / (2 * sigma * sigma)) / (2 * M_PI * sigma * sigma);
            kernel[(i + halfKernel) * kernelSize + (j + halfKernel)] = weight;
            sum += weight;
        }
    }
    // Normalize the kernel
    for (double &weight : kernel) {
        weight /= sum;
    }

    // Apply the Gaussian kernel to each pixel
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            for (int c = 0; c < channels; ++c) {
                double value = 0.0;
                for (int ky = -halfKernel; ky <= halfKernel; ++ky) {
                    for (int kx = -halfKernel; kx <= halfKernel; ++kx) {
                        int nx = x + kx;
                        int ny = y + ky;
                        if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                            int index = (ny * width + nx) * channels + c;
                            value += data[index] * kernel[(ky + halfKernel) * kernelSize + (kx + halfKernel)];
                        }
                    }
                }
                newData[y * width * channels + x * channels + c] = static_cast<unsigned char>(value);
            }
        }
    }

    std::copy(newData.begin(), newData.end(), data);
}
