#include "ImageBlur.h"
#include <algorithm>
#include <cmath>
#include <numeric>
#include <vector>

ImageBlur::ImageBlur(BlurType type, int kernelSize) : blurType(type), kernelSize(kernelSize) {}

ImageBlur::~ImageBlur() {}

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
