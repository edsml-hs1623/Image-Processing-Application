#include "ImageBlurTest.h"
#include "Image.h"
#include "ImageBlur.h"
#include "ColourCorrection.h"
#include <iostream>
#include <numeric>
#include <cmath>

void ImageBlurTest::run(int testType) {
    ImageBlurTestType specificTestType = static_cast<ImageBlurTestType>(testType);
    switch (specificTestType) {
        case TestMedianBlur:
            testMedianBlur();
            break;
        case TestBoxBlur:
            testBoxBlur();
            break;
        case TestGaussianBlur:
            testGaussianBlur();
            break;
        default:
            std::cerr << "Unknown blur test type provided." << std::endl;
            break;
    }
}

double ImageBlurTest::calculateNoiseLevel(const Image& image) {
    const unsigned char* data = image.getData();
    int width = image.getWidth();
    int height = image.getHeight();
    int channels = image.getChannels();
    int size = width * height * channels;

    int noisePixels = 0;
    for (int i = 0; i < size; i += channels) {
        // For grayscale or colored images, considering only the first channel
        if (data[i] <= 0 || data[i] >= 255) {
            ++noisePixels;
        }
    }

    return static_cast<double>(noisePixels) / (width * height);
}

void ImageBlurTest::testMedianBlur() {
    Image image;
    if (!image.loadImage("../Images/vh_anatomy_sp15.png")) {
        std::cerr << "Failed to load clear image for median blur test." << std::endl;
        return;
    }

    double originalNoiseLevel = calculateNoiseLevel(image);

    ImageBlur medianBlur(Median, 3); // Using a 3x3 kernel for example
    medianBlur.apply(image);

    // Calculate noise level of the processed image
    double blurredNoiseLevel = calculateNoiseLevel(image);

    // Verify if the noise level significantly decreased
    if (blurredNoiseLevel < originalNoiseLevel) {
        std::cout << "Median Blur Test Passed: Noise level decreased by ";
        std::cout << originalNoiseLevel - blurredNoiseLevel << std::endl;
    } else {
        std::cerr << "Median Blur Test Failed: Noise level did not decrease significantly." << std::endl;
    }
}

double ImageBlurTest::calculateStdDev(const Image& image) {
    const unsigned char* data = image.getData();
    int width = image.getWidth();
    int height = image.getHeight();
    int channels = image.getChannels();
    std::vector<double> pixelValues;

    for (int i = 0; i < width * height * channels; ++i) {
        pixelValues.push_back(static_cast<double>(data[i]));
    }
    double mean = std::accumulate(pixelValues.begin(), pixelValues.end(), 0.0) / pixelValues.size();
    double varianceSum = 0.0;
    for (double value : pixelValues) {
        varianceSum += (value - mean) * (value - mean);
    }
    double variance = varianceSum / pixelValues.size();

    return sqrt(variance);
}

void ImageBlurTest::testBoxBlur() {
    Image image;
    if (!image.loadImage("../Images/gracehopper.png")) {
        std::cerr << "Failed to load image for box blur test." << std::endl;
        return;
    }

    // Calculate the standard deviation of the original image
    double originalStdDev = calculateStdDev(image);

    // Apply box blur, using a 3x3 kernel for example
    ImageBlur boxBlur(Box, 3);
    boxBlur.apply(image);

    // Calculate the standard deviation of the blurred image
    double blurredStdDev = calculateStdDev(image);

    // Verify the blur effect: the standard deviation of the blurred image should be smaller than that of the original image
    if (blurredStdDev < originalStdDev) {
        std::cout << "Box Blur Test Passed: The standard deviation decreased from "
                  << originalStdDev << " to " << blurredStdDev << ", indicating the image is smoother." << std::endl;
    } else {
        std::cerr << "Box Blur Test Failed: Expected a lower standard deviation in the blurred image.\n";
    }
}

void ImageBlurTest::testGaussianBlur() {
    Image image;
    // Ensure the test image path is correct
    if (!image.loadImage("../Images/gracehopper.png")) {
        std::cerr << "Failed to load image for Gaussian blur test." << std::endl;
        return;
    }

    // Calculate the standard deviation of the original image
    double originalStdDev = calculateStdDev(image);

    // Apply Gaussian blur, using a 5x5 kernel for example
    ImageBlur gaussianBlur(Gaussian, 5);
    gaussianBlur.apply(image);

    // Calculate the standard deviation of the blurred image
    double blurredStdDev = calculateStdDev(image);

    // Verify the blur effect: the standard deviation of the blurred image should be smaller than that of the original image
    if (blurredStdDev < originalStdDev) {
        std::cout << "Gaussian Blur Test Passed: The standard deviation decreased from "
                  << originalStdDev << " to " << blurredStdDev << ", indicating the image is smoother." << std::endl;
    } else {
        std::cerr << "Gaussian Blur Test Failed: The standard deviation did not decrease significantly." << std::endl;
    }
}