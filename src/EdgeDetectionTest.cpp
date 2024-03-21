#include "EdgeDetectionTest.h"
#include <iostream>
#include <cmath>
#include <vector>

void EdgeDetectionTest::run(int testType) {
    EdgeDetectionTestType specificTestType = static_cast<EdgeDetectionTestType>(testType);
    switch (specificTestType) {
        case TestSobel:
            testSobel();
            break;
        case TestPrewitt:
            testPrewitt();
            break;
        case TestScharr:
            testScharr();
            break;
        case TestRobertsCross:
            testRobertsCross();
            break;
        default:
            std::cerr << "Unknown edge detection test type provided." << std::endl;
            break;
    }
}


double calculateAverageGradientMagnitude(const Image& image) {
    int width = image.getWidth();
    int height = image.getHeight();
    int channels = image.getChannels();
    const unsigned char* data = image.getData();

    double sum = 0.0;
    int count = 0;

    // Iterate over each pixel in the image
    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            // For colored images, calculate gradients for each RGB channel and then take average
            double gxTotal = 0.0;
            double gyTotal = 0.0;
            for (int c = 0; c < channels; ++c) {
                int index = (y * width + x) * channels + c;
                double gx = (-1 * data[index - width * channels - channels + c] +
                             1 * data[index - width * channels + channels + c] +
                             -2 * data[index - channels + c] +
                             2 * data[index + channels + c] +
                             -1 * data[index + width * channels - channels + c] +
                             1 * data[index + width * channels + channels + c]) / 4.0;
                double gy = (-1 * data[index - width * channels - channels + c] -
                             2 * data[index - width * channels + c] -
                             1 * data[index - width * channels + channels + c] +
                             1 * data[index + width * channels - channels + c] +
                             2 * data[index + width * channels + c] +
                             1 * data[index + width * channels + channels + c]) / 4.0;
                gxTotal += gx;
                gyTotal += gy;
            }
            double magnitude = sqrt((gxTotal / channels) * (gxTotal / channels) +
                                    (gyTotal / channels) * (gyTotal / channels));
            sum += magnitude;
            ++count;
        }
    }

    return sum / count;
}

void EdgeDetectionTest::testSobel() {
    Image image;
    if (!image.loadImage("../Images/gracehopper.png")) {
        std::cerr << "Failed to load image for Sobel test." << std::endl;
        return;
    }

    // Calculate average gradient magnitude of original image
    double originalAvgGradient = calculateAverageGradientMagnitude(image);

    // Apply Sobel edge detection
    EdgeDetection edgeDetection(Sobel);
    edgeDetection.apply(image);

    // Calculate average gradient magnitude of image after applying Sobel
    double sobelAvgGradient = calculateAverageGradientMagnitude(image);

    // Compare change in gradient magnitude before and after Sobel
    if (sobelAvgGradient > originalAvgGradient) {
        std::cout << "Sobel Edge Detection Test Passed: Average gradient magnitude increased from "
                  << originalAvgGradient << " to " << sobelAvgGradient << "." << std::endl;
    } else {
        std::cerr << "Sobel Edge Detection Test Failed: Average gradient magnitude did not increase as expected." << std::endl;
    }
}

void EdgeDetectionTest::testPrewitt() {
    Image image;
    if (!image.loadImage("../Images/gracehopper.png")) {
        std::cerr << "Failed to load image for Prewitt test." << std::endl;
        return;
    }

    // Calculate average gradient magnitude of original image
    double originalAvgGradient = calculateAverageGradientMagnitude(image);

    // Apply Prewitt edge detection
    EdgeDetection edgeDetection(Prewitt);
    edgeDetection.apply(image);

    // Calculate average gradient magnitude of image after applying Prewitt
    double prewittAvgGradient = calculateAverageGradientMagnitude(image);

    // Compare change in gradient magnitude before and after Prewitt
    if (prewittAvgGradient > originalAvgGradient) {
        std::cout << "Prewitt Edge Detection Test Passed: Average gradient magnitude increased from "
                  << originalAvgGradient << " to " << prewittAvgGradient << "." << std::endl;
    } else {
        std::cerr << "Prewitt Edge Detection Test Failed: Average gradient magnitude did not increase as expected." << std::endl;
    }
}

void EdgeDetectionTest::testScharr() {
    Image image;
    if (!image.loadImage("../Images/gracehopper.png")) {
        std::cerr << "Failed to load image for Scharr test." << std::endl;
        return;
    }

    // Calculate average gradient magnitude of original image
    double originalAvgGradient = calculateAverageGradientMagnitude(image);

    // Apply Scharr edge detection
    EdgeDetection edgeDetection(Scharr);
    edgeDetection.apply(image);

    // Calculate average gradient magnitude of image after applying Scharr
    double scharrAvgGradient = calculateAverageGradientMagnitude(image);

    // Compare change in gradient magnitude before and after Scharr
    if (scharrAvgGradient > originalAvgGradient) {
        std::cout << "Scharr Edge Detection Test Passed: Average gradient magnitude increased from "
                  << originalAvgGradient << " to " << scharrAvgGradient << "." << std::endl;
    } else {
        std::cerr << "Scharr Edge Detection Test Failed: Average gradient magnitude did not increase as expected, indicating a potential issue with edge enhancement." << std::endl;
    }
}


void EdgeDetectionTest::testRobertsCross() {
    Image image;
    if (!image.loadImage("../Images/vh_anatomy_sp15.png")) {
        std::cerr << "Failed to load image for Roberts Cross test." << std::endl;
        return;
    }

    // Calculate average gradient magnitude of original image
    double originalAvgGradient = calculateAverageGradientMagnitude(image);

    // Apply Roberts Cross edge detection
    EdgeDetection edgeDetection(RobertsCross);
    edgeDetection.apply(image);

    // Calculate average gradient magnitude of image after applying Roberts Cross
    double robertsCrossAvgGradient = calculateAverageGradientMagnitude(image);

    // Compare change in gradient magnitude before and after Roberts Cross
    if (robertsCrossAvgGradient > originalAvgGradient) {
        std::cout << "Roberts Cross Edge Detection Test Passed: Average gradient magnitude increased from "
                  << originalAvgGradient << " to " << robertsCrossAvgGradient << "." << std::endl;
    } else {
        std::cerr << "Roberts Cross Edge Detection Test Failed: Average gradient magnitude did not increase as expected, indicating a potential issue with edge enhancement." << originalAvgGradient << " to " << robertsCrossAvgGradient << "."<< std::endl;
    }
}