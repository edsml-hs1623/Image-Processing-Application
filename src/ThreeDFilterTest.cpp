#include "ThreeDFilterTest.h"
#include "Volume.h"
#include "ThreeDFilter.h"
#include <iostream>
#include <cmath>
#include <numeric>

void ThreeDFilterTest::run(int testType) {
    // Use a switch statement to execute only the selected tests
    switch (testType) {
        case TestGaussian:
            testGaussianBlur();
            break;
        case TestMedian:
            testMedianBlur();
            break;
        default:
            std::cerr << "Unknown filter test type provided: " << testType << std::endl;
            break;
    }
}
double ThreeDFilterTest::calculateStdDev(const Volume& volume) {
    const auto& data = volume.getData();
    std::vector<double> intensities;
    for (const auto& slice : data) {
        for (unsigned char pixel : slice) {
            intensities.push_back(static_cast<double>(pixel));
        }
    }

    double mean = std::accumulate(intensities.begin(), intensities.end(), 0.0) / intensities.size();
    double varianceSum = 0.0;
    for (double value : intensities) {
        varianceSum += (value - mean) * (value - mean);
    }
    return std::sqrt(varianceSum / intensities.size());
}

void ThreeDFilterTest::testGaussianBlur() {
    Volume volume;
    // Load volume data - ensure this is correctly set up
    if (!volume.loadVolume("../Scans/confuciusornis")) {
        std::cerr << "Failed to load volume for Gaussian blur test." << std::endl;
        return;
    }

    double originalStdDev = calculateStdDev(volume);

    ThreeDFilter::gaussianBlur(volume, 3, 2.0); // Example kernel size and sigma
    double blurredStdDev = calculateStdDev(volume);

    if (blurredStdDev < originalStdDev) {
        std::cout << "Gaussian Blur Test Passed: Standard deviation decreased from "
                  << originalStdDev << " to " << blurredStdDev << std::endl;
    } else {
        std::cerr << "Gaussian Blur Test Failed: Standard deviation did not decrease." << std::endl;
    }
}

void ThreeDFilterTest::testMedianBlur() {
    Volume volume;
    // Load volume data - ensure this is correctly set up
    if (!volume.loadVolume("../Scans/confuciusornis")) {
        std::cerr << "Failed to load volume for Median blur test." << std::endl;
        return;
    }

    double originalStdDev = calculateStdDev(volume);

    ThreeDFilter::medianBlur(volume, 3); // Example kernel size
    double blurredStdDev = calculateStdDev(volume);

    if (blurredStdDev < originalStdDev) {
        std::cout << "Median Blur Test Passed: Standard deviation decreased from "
                  << originalStdDev << " to " << blurredStdDev << std::endl;
    } else {
        std::cerr << "Median Blur Test Failed: Standard deviation did not decrease." << std::endl;
    }
}