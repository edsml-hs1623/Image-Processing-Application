// ColourCorrectionTest.cpp
#include "ColourCorrectionTest.h"
#include "ColourCorrection.h"
#include "Image.h"
#include <iostream>
#include <numeric>
#include <cmath>
#include <vector>
#include <cstring>

void ColourCorrectionTest::run(int testType) {
    ColourCorrectionTestType specificTestType = static_cast<ColourCorrectionTestType>(testType);
    switch (specificTestType) {
        case TestGrayscale:
            testGrayscale();
            break;
        case TestBrightnessAdjustment:
            testBrightnessAdjustment();
            break;
        case TestHistogramEqualization:
            testHistogramEqualization();
            break;
        case TestThresholding:
            testThresholding();
            break;
        case TestSaltAndPepperNoise:
            testSaltAndPepperNoise();
            break;
        default:
            std::cerr << "Unknown test type provided." << std::endl;
            break;
    }
}

// Tests converting an image to grayscale.
void ColourCorrectionTest::testGrayscale() {
    Image image;
    if (!image.loadImage("../Images/gracehopper.png")) {
        std::cerr << "Failed to load image for grayscale test." << std::endl;
        return;
    }

    ColourCorrection grayscaleFilter(Grayscale);
    grayscaleFilter.apply(image);

    if (image.getChannels() == 1) {
        std::cout << "Grayscale test passed: Image only has 1 channel." << std::endl;
    } else {
        std::cerr << "Grayscale test failed: Image is not in grayscale." << std::endl;
    }
}

// Computes the average brightness of an image.
double computeAverageBrightness(const unsigned char* data, int img_size, int channels) {
    long long totalBrightness = 0;
    for (int i = 0; i < img_size; i += channels) {
        totalBrightness += 0.2126 * data[i] + 0.7152 * data[i + 1] + 0.0722 * data[i + 2]; // Assuming RGB channels
    }
    return totalBrightness / (double)(img_size / channels);
}

// Tests adjusting the brightness of an image.
void ColourCorrectionTest::testBrightnessAdjustment() {
    Image image;
    if (!image.loadImage("../Images/gracehopper.png")) {
        std::cerr << "Failed to load image for brightness adjustment test." << std::endl;
        return;
    }

    int testBrightness = 50; // Define a test brightness value.

    // Get original image data and its properties.
    unsigned char* originalData = new unsigned char[image.getWidth() * image.getHeight() * image.getChannels()];
    memcpy(originalData, image.getData(), image.getWidth() * image.getHeight() * image.getChannels());
    int width = image.getWidth();
    int height = image.getHeight();
    int channels = image.getChannels();

    // Create a ColourCorrection instance and apply brightness adjustment.
    ColourCorrection brightnessAdjustment(BrightnessAdjust, testBrightness);
    brightnessAdjustment.apply(image);  // Assuming apply method can directly apply to Image object and update its data.

    // Get adjusted image data.
    unsigned char* adjustedData = image.getData();

    // Compute average brightness of original and adjusted images.
    double originalAvgBrightness = computeAverageBrightness(originalData, width * height * channels, channels);
    double adjustedAvgBrightness = computeAverageBrightness(adjustedData, width * height * channels, channels);

    // Compare average brightness to verify if brightness has increased.
    if (adjustedAvgBrightness > originalAvgBrightness) {
        std::cout << "Brightness adjustment test passed: The average brightness is higher." << std::endl;
    } else {
        std::cerr << "Brightness adjustment test failed: Image brightness did not increase as expected." << std::endl;
    }

    // Clean up allocated memory.
    delete[] originalData;
}

// Calculates the histogram of an image.
std::vector<int> calculateHistogram(const unsigned char* data, int size, int channels) {
    std::vector<int> histogram(256, 0);
    for (int i = 0; i < size; i += channels) {
        histogram[data[i]]++;
    }
    return histogram;
}

// Calculates the standard deviation of a histogram to evaluate the distribution's width.
double calculateStdDev(const std::vector<int>& histogram, int totalPixels) {
    double mean = std::accumulate(histogram.begin(), histogram.end(), 0.0) / histogram.size();
    double variance = 0.0;
    for (int i = 0; i < histogram.size(); ++i) {
        variance += (i - mean) * (i - mean) * histogram[i];
    }
    variance /= totalPixels;
    return std::sqrt(variance);
}

void ColourCorrectionTest::testHistogramEqualization() {
    Image Image;
    Image.loadImage("../Images/stinkbug.png");

    auto originalHistogram = calculateHistogram(Image.getData(), Image.getWidth() * Image.getHeight(), Image.getChannels());
    auto originalStdDev = calculateStdDev(originalHistogram, Image.getWidth() * Image.getHeight());

    // Apply histogram equalization
    ColorSpace colorspace = ColorSpace::HSL; // or ColorSpace::HSV
    ColourCorrection histogramEqualizationFilter(HistogramEqualization, 0, colorspace); // Assume using HSV color space
    histogramEqualizationFilter.apply(Image);

    auto equalizedHistogram = calculateHistogram(Image.getData(), Image.getWidth() * Image.getHeight(), Image.getChannels());
    auto equalizedStdDev = calculateStdDev(equalizedHistogram, Image.getWidth() * Image.getHeight());


    if (equalizedStdDev > originalStdDev) {
        std::cout << "Histogram equalization test passed: Distribution is wider." << std::endl;
    } else {
        std::cerr << "Histogram equalization test failed: Distribution did not significantly improve." << std::endl;
    }
}

void ColourCorrectionTest::testThresholding() {
    Image image;
    if (!image.loadImage("../Images/gracehopper.png")) {
        std::cerr << "Failed to load image for thresholding test." << std::endl;
        return;
    }

    unsigned char threshold = 128; // Define threshold
    ColorSpace colorspace = ColorSpace::HSL; // or ColorSpace::HSV
    ColourCorrection thresholding(Thresholding, threshold,colorspace);

    // Apply thresholding
    thresholding.apply(image);

    // Get processed image data
    unsigned char* data = image.getData();
    int width = image.getWidth();
    int height = image.getHeight();
    int channels = image.getChannels();
    int img_size = width * height * channels;

    // Validate thresholding result
    bool isThresholdingCorrect = true;
    for (int i = 0; i < img_size; i += channels) {
        if (channels >= 3) {
            // Assume thresholding is based on brightness or value
            float channelValue = (colorspace == ColorSpace::HSL) ?
                                 rgbToHsl(data[i], data[i + 1], data[i + 2]).l * 255 :
                                 rgbToHsv(data[i], data[i + 1], data[i + 2]).v * 255;

            if (channelValue != 0 && channelValue != 255) {
                isThresholdingCorrect = false;
                break;
            }
        } else {
            // For grayscale images
            if (data[i] != 0 && data[i] != 255) {
                isThresholdingCorrect = false;
                break;
            }
        }
        // Ignore processing of Alpha channel
    }

    if (isThresholdingCorrect) {
        std::cout << "Thresholding test passed: All pixels are black and white." << std::endl;
    } else {
        std::cerr << "Thresholding test failed: Image thresholding did not work as expected." << std::endl;
    }
}

// Calculate percentage of black and white pixels in the image
float calculateBlackWhitePercentage(const unsigned char* data, int width, int height, int channels) {
    int size = width * height * channels;
    int bwCount = 0; // Count of black and white pixels

    for (int i = 0; i < size; i += channels) {
        // Check if each pixel is black or white
        bool isBW = true;
        for (int ch = 0; ch < channels; ++ch) {
            if (channels == 4 && ch == 3) continue; // Ignore alpha channel
            if (data[i + ch] != 0 && data[i + ch] != 255) {
                isBW = false;
                break;
            }
        }
        if (isBW) bwCount++;
    }

    // Return percentage of black and white pixels
    return static_cast<float>(bwCount) / (width * height) * 100.0f;
}

// Modify function to return count of black and white pixels
int calculateBlackWhitePixelCount(const unsigned char* data, int width, int height, int channels) {
    int size = width * height * channels;
    int bwCount = 0; // Count of black and white pixels

    for (int i = 0; i < size; i += channels) {
        // Check if each pixel is black or white
        bool isBW = true;
        for (int ch = 0; ch < channels; ++ch) {
            if (channels == 4 && ch == 3) continue; // Ignore alpha channel
            if (data[i + ch] != 0 && data[i + ch] != 255) {
                isBW = false;
                break;
            }
        }
        if (isBW) bwCount++;
    }

    return bwCount;
}

void ColourCorrectionTest::testSaltAndPepperNoise() {
    Image image;
    if (!image.loadImage("../Images/gracehopper.png")) {
        std::cerr << "Failed to load image for salt and pepper noise test." << std::endl;
        return;
    }

    int originalBWCount = calculateBlackWhitePixelCount(image.getData(), image.getWidth(), image.getHeight(), image.getChannels());
    int totalPixels = image.getWidth() * image.getHeight();
    float noisePercentage = 5.0f; // Expected noise percentage to add

    ColourCorrection noise(SaltPepperNoise, static_cast<int>(noisePercentage));
    noise.apply(image);

    int noisyBWCount = calculateBlackWhitePixelCount(image.getData(), image.getWidth(), image.getHeight(), image.getChannels());
    int expectedIncrease = static_cast<int>(totalPixels * (noisePercentage / 100.0f)); // Expected increase in black and white pixels count
    int actualIncrease = noisyBWCount - originalBWCount; // Actual increase in black and white pixels count

    // Check if actual increase in black and white pixels count is close to expected
    if (abs(actualIncrease - expectedIncrease) <= totalPixels * 0.01) { // Allowing 1% error margin
        std::cout << "Salt and pepper noise test passed: Actual increase is close to the expected." << std::endl;
    } else {
        std::cerr << "Salt and pepper noise test failed: Actual increase does not match the expected." << std::endl
                  << "Expected increase: " << expectedIncrease << " pixels, Actual increase: " << actualIncrease << " pixels." << std::endl;
    }
}