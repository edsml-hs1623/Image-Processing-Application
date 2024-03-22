/**
 * @file ColourCorrection.cpp
 *
 * @brief This module provides comprehensive functionality for color correction
 *        in digital images. It includes a range of techniques for manipulating
 *        and enhancing the colors of an image according to specified parameters.
 *
 * The ColourCorrection module is designed to support various common color correction
 * tasks, including grayscale conversion, brightness adjustment, histogram equalization,
 * thresholding, and applying salt and pepper noise. Specially for histogram equalization
 * and thresholding, it is versatile in its application, supporting color space convertion
 * from RGB to both HSV and HSL.
 *
 * Key Features:
 *   - Grayscale Conversion: Transform color images into grayscale.
 *   - Brightness Adjustment: Modify the brightness of an image.
 *   - Histogram Equalization: Improve contrast using histogram equalization.
 *   - Thresholding: Apply binary thresholding to images for segmentation purposes.
 *   - Salt and Pepper Noise: Introduce noise to an image for testing and simulation.
 *
 * Usage:
 *   Image img = ...; // An instance of the Image class
 *   ColourCorrection correction(ColourCorrection::Thresholding, 127, ColorSpace::HSL);
 *   correction.apply(img); // Applies thresholding to 'img'
 *
 * @note The module expects a compatible Image class for processing. It is essential
 *       that the input images are properly formatted and that the Image class supports
 *       the required color spaces and data manipulations.
 * @author acse-yw3523,edsml-lwk16, acse-ad2123, 
 *         edsml-hs1623, acse-xg1123, edsml-st2923,
 *         Group: selection sort.
 */

#include "ColourCorrection.h"
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring> // for std::memcpy
#include <cstdlib> // for std::rand, std::srand
#include <ctime>   // for std::time
#include <iostream>


/**
 * Constructor: Constructs a ColourCorrection object with a specified type, parameter, and color space.
 *
 * @param type The type of color correction to apply (e.g., grayscale, brightness adjustment).
 * @param param An integer parameter used for certain types of corrections (e.g., brightness value).
 * @param colorSpace The color space in which the correction is to be applied (e.g., RGB, HSV).
 */
ColourCorrection::ColourCorrection(ColourCorrectionType type, int param, ColorSpace colorSpace)
    : correctionType(type), parameter(param), colorSpace(colorSpace) {
}

/**
 * Destructor: Destructor for ColourCorrection. Cleans up any resources used.
 */
ColourCorrection::~ColourCorrection() {}

/**
 * Applies the specified color correction to an image.
 *
 * @param image The image to which the color correction will be applied.
 */
void ColourCorrection::apply(Image& image) {
    unsigned char* imgData = image.getData();
    int width = image.getWidth();
    int height = image.getHeight();
    int channels = image.getChannels();

    unsigned char* processedData = nullptr;
    switch (correctionType) {
    case Grayscale:
        processedData = applyGrayscale(imgData, width, height, channels);
        break;
    case BrightnessAdjust:
        processedData = adjustBrightness(imgData, width, height, channels, parameter);
        break;
    case HistogramEqualization:
        processedData = histogramEqualisation(imgData, width, height, channels, colorSpace);
        break;
    case Thresholding:
        processedData = applyThresholding(imgData, width, height, channels, static_cast<unsigned char>(parameter), colorSpace);
        break;
    case SaltPepperNoise:
        processedData = saltAndPepperNoise(imgData, width, height, channels, static_cast<float>(parameter));
        break;
    default:
        std::cerr << "Unsupported colour correction type" << std::endl;
        return;
    }

    if (correctionType == Grayscale) {
        // Grayscale will change the number of channels to 1
        image.updateData(processedData, width, height, 1);
    }
    else {
        // For other corrections, the number of channels remains the same
        std::memcpy(imgData, processedData, width * height * channels);
        delete[] processedData;
    }
}

/**
 * Converts a color image to grayscale.
 *
 * @param image Pointer to the image data.
 * @param width Width of the image.
 * @param height Height of the image.
 * @param channels Number of color channels in the image.
 * @return Pointer to the grayscale image data.
 */
unsigned char* ColourCorrection::applyGrayscale(unsigned char* image, int width, int height, int channels) {
    int img_size = width * height; // Total number of pixels
    unsigned char* gray_image = new unsigned char[img_size]; // Allocate memory for grayscale image

    for (int i = 0, j = 0; i < img_size * channels; i += channels, ++j) {
        // Calculate the grayscale value
        float gray_value = 0.2126f * image[i] + 0.7152f * image[i + 1] + 0.0722f * image[i + 2];
        // Assign the calculated grayscale value
        gray_image[j] = static_cast<unsigned char>(gray_value);
    }

    return gray_image;
}

/**
 * Adjusts the brightness of an image.
 *
 * @param image Pointer to the image data.
 * @param width Width of the image.
 * @param height Height of the image.
 * @param channels Number of color channels in the image.
 * @param brightness The brightness adjustment value (-256 for automatic adjustment).
 * @return Pointer to the brightness-adjusted image data.
 */
unsigned char* ColourCorrection::adjustBrightness(unsigned char* image, int width, int height, int channels, int brightness) {
    int img_size = width * height * channels;
    unsigned char* adjusted_image = new unsigned char[img_size];

    // Automatic brightness adjustment (optional)
    if (brightness == -256) {
        long totalBrightness = 0;
        for (int i = 0; i < img_size; i += channels) {
            // Calculate the perceived brightness
            totalBrightness += 0.2126 * image[i] + 0.7152 * image[i + 1] + 0.0722 * image[i + 2];
        }
        brightness = 128 - static_cast<int>(totalBrightness / (width * height));
    }

    // Adjust brightness
    for (int i = 0; i < img_size; i++) {
        int newValue = static_cast<int>(image[i]) + brightness;
        adjusted_image[i] = static_cast<unsigned char>(std::max(0, std::min(255, newValue)));
    }

    return adjusted_image;
}

// Utility functions for RGB to HSV conversion and back
HSV rgbToHsv(unsigned char r, unsigned char g, unsigned char b) {
    HSV hsv;
    float rd = static_cast<float>(r) / 255.0;
    float gd = static_cast<float>(g) / 255.0;
    float bd = static_cast<float>(b) / 255.0;
    float max = std::max(rd, std::max(gd, bd)), min = std::min(rd, std::min(gd, bd));
    hsv.v = max;

    float d = max - min;
    hsv.s = max == 0 ? 0 : d / max;

    if (max == min) {
        hsv.h = 0; // achromatic
    }
    else {
        if (max == rd) {
            hsv.h = (gd - bd) / d + (gd < bd ? 6 : 0);
        }
        else if (max == gd) {
            hsv.h = (bd - rd) / d + 2;
        }
        else if (max == bd) {
            hsv.h = (rd - gd) / d + 4;
        }
        hsv.h /= 6;
    }

    return hsv;
}

void hsvToRgb(HSV hsv, unsigned char& r, unsigned char& g, unsigned char& b) {
    float r_f, g_f, b_f;
    int i = static_cast<int>(hsv.h * 6);
    float f = hsv.h * 6 - i;
    float p = hsv.v * (1 - hsv.s);
    float q = hsv.v * (1 - f * hsv.s);
    float t = hsv.v * (1 - (1 - f) * hsv.s);

    switch (i % 6) {
    case 0: r_f = hsv.v, g_f = t, b_f = p; break;
    case 1: r_f = q, g_f = hsv.v, b_f = p; break;
    case 2: r_f = p, g_f = hsv.v, b_f = t; break;
    case 3: r_f = p, g_f = q, b_f = hsv.v; break;
    case 4: r_f = t, g_f = p, b_f = hsv.v; break;
    case 5: r_f = hsv.v, g_f = p, b_f = q; break;
    }

    r = static_cast<unsigned char>(r_f * 255.0);
    g = static_cast<unsigned char>(g_f * 255.0);
    b = static_cast<unsigned char>(b_f * 255.0);
}

// Utility functions for RGB to HSL conversion and back
HSL rgbToHsl(unsigned char r, unsigned char g, unsigned char b) {
    float rd = static_cast<float>(r) / 255.0f;
    float gd = static_cast<float>(g) / 255.0f;
    float bd = static_cast<float>(b) / 255.0f;

    float max = std::max(rd, std::max(gd, bd));
    float min = std::min(rd, std::min(gd, bd));
    HSL hsl;
    hsl.l = (max + min) / 2;

    if (max == min) {
        hsl.h = hsl.s = 0; // achromatic
    }
    else {
        float d = max - min;
        hsl.s = hsl.l > 0.5f ? d / (2 - max - min) : d / (max + min);

        if (max == rd) {
            hsl.h = (gd - bd) / d + (gd < bd ? 6 : 0);
        }
        else if (max == gd) {
            hsl.h = (bd - rd) / d + 2;
        }
        else if (max == bd) {
            hsl.h = (rd - gd) / d + 4;
        }
        hsl.h /= 6;
    }

    return hsl;
}

void hslToRgb(HSL hsl, unsigned char& r, unsigned char& g, unsigned char& b) {
    auto hueToRgb = [](float p, float q, float t) {
        if (t < 0) t += 1;
        if (t > 1) t -= 1;
        if (t < 1.0f / 6) return p + (q - p) * 6 * t;
        if (t < 1.0f / 2) return q;
        if (t < 2.0f / 3) return p + (q - p) * (2.0f / 3 - t) * 6;
        return p;
        };

    float q = hsl.l < 0.5f ? hsl.l * (1 + hsl.s) : hsl.l + hsl.s - hsl.l * hsl.s;
    float p = 2 * hsl.l - q;
    float r_f = hueToRgb(p, q, hsl.h + 1.0f / 3);
    float g_f = hueToRgb(p, q, hsl.h);
    float b_f = hueToRgb(p, q, hsl.h - 1.0f / 3);

    r = static_cast<unsigned char>(r_f * 255.0f);
    g = static_cast<unsigned char>(g_f * 255.0f);
    b = static_cast<unsigned char>(b_f * 255.0f);
}

/**
 * Performs histogram equalization on an image.
 *
 * @param image Pointer to the image data.
 * @param width Width of the image.
 * @param height Height of the image.
 * @param channels Number of color channels in the image.
 * @param colorSpace The color space to be used for equalization (e.g., HSV, HSL).
 * @return Pointer to the histogram-equalized image data.
 */

unsigned char* ColourCorrection::histogramEqualisation(unsigned char* image, int width, int height, int channels, ColorSpace colorSpace) {
    int img_size = width * height * channels;
    unsigned char* equalised_image = new unsigned char[img_size];

    if (channels == 1) { // Grayscale case
        // Compute histogram
        std::vector<int> histogram(256, 0);
        for (int i = 0; i < img_size; i++) {
            histogram[image[i]]++;
        }

        // Compute cumulative distribution function (CDF)
        std::vector<int> cdf(256, 0);
        cdf[0] = histogram[0];
        for (int i = 1; i < 256; i++) {
            cdf[i] = cdf[i - 1] + histogram[i];
        }

        // Normalize CDF and equalize image
        float cdf_min = cdf[0];
        for (int i = 0; i < img_size; i++) {
            equalised_image[i] = static_cast<unsigned char>((cdf[image[i]] - cdf_min) / (width * height - cdf_min) * 255);
        }
    }
    else if (channels == 3 || channels == 4) {  // RGB case
        // Convert to HSV and equalize the V channel
        std::vector<float> color_channel(width * height);
        if (colorSpace == ColorSpace::HSV) {
            for (int i = 0; i < img_size; i += channels) {
                HSV hsv = rgbToHsv(image[i], image[i + 1], image[i + 2]);
                color_channel[i / channels] = hsv.v;
            }

            // Histogram equalization on the V channel
            std::vector<int> v_histogram(256, 0);
            for (float v : color_channel) {
                int v_int = static_cast<int>(v * 255);
                v_histogram[v_int]++;
            }

            std::vector<int> v_cdf(256, 0);
            v_cdf[0] = v_histogram[0];
            for (int i = 1; i < 256; i++) {
                v_cdf[i] = v_cdf[i - 1] + v_histogram[i];
            }

            float cdf_min = v_cdf[0];
            int total_pixels = width * height;
            for (int i = 0; i < total_pixels; i++) {
                int v_int = static_cast<int>(color_channel[i] * 255);
                float equalized_v = static_cast<float>(v_cdf[v_int] - cdf_min) / (total_pixels - cdf_min) * 255;
                color_channel[i] = equalized_v / 255.0f; // Normalize back to [0, 1] range
            }

            // Convert equalized V channel back to RGB
            for (int i = 0; i < img_size; i += channels) {
                HSV hsv = rgbToHsv(image[i], image[i + 1], image[i + 2]);
                hsv.v = color_channel[i / channels];
                hsvToRgb(hsv, equalised_image[i], equalised_image[i + 1], equalised_image[i + 2]);

                // Copy alpha channel for RGBA images
                if (channels == 4) {
                    equalised_image[i + 3] = image[i + 3];
                }
            }

        }
        else if (colorSpace == ColorSpace::HSL) {
            for (int i = 0; i < img_size; i += channels) {
                HSL hsl = rgbToHsl(image[i], image[i + 1], image[i + 2]);
                color_channel[i / channels] = hsl.l;
            }

            // Histogram equalization on the L channel
            std::vector<int> l_histogram(256, 0);
            for (float l : color_channel) {
                int l_int = static_cast<int>(l * 255);
                l_histogram[l_int]++;
            }

            std::vector<int> l_cdf(256, 0);
            l_cdf[0] = l_histogram[0];
            for (int i = 1; i < 256; i++) {
                l_cdf[i] = l_cdf[i - 1] + l_histogram[i];
            }

            float cdf_min = *std::min_element(std::next(l_cdf.begin()), l_cdf.end()); // Find the min in the CDF starting from the second element
            int total_pixels = width * height;
            for (int i = 0; i < total_pixels; i++) {
                int l_int = static_cast<int>(color_channel[i] * 255);
                float equalized_l = static_cast<float>(l_cdf[l_int] - cdf_min) / (total_pixels - cdf_min) * 255;
                color_channel[i] = equalized_l / 255.0f; // Normalize back to [0, 1] range
            }


            // Convert equalized L channel back to RGB
            for (int i = 0; i < img_size; i += channels) {
                HSL hsl = rgbToHsl(image[i], image[i + 1], image[i + 2]);
                hsl.l = color_channel[i / channels]; // Use the equalized lightness value
                hslToRgb(hsl, equalised_image[i], equalised_image[i + 1], equalised_image[i + 2]);

                // Copy alpha channel for RGBA images
                if (channels == 4) {
                    equalised_image[i + 3] = image[i + 3];
                }
            }
        }
    }

    return equalised_image;
}

/**
 * Applies thresholding to an image.
 *
 * @param image Pointer to the image data.
 * @param width Width of the image.
 * @param height Height of the image.
 * @param channels Number of color channels in the image.
 * @param threshold The threshold value for binarization.
 * @param colorSpace The color space used for thresholding (e.g., HSV, HSL).
 * @return Pointer to the thresholded image data.
 */
unsigned char* ColourCorrection::applyThresholding(unsigned char* image, int width, int height, int channels, unsigned char threshold, ColorSpace colorSpace) {
    int img_size = width * height * channels;
    unsigned char* thresholded_image = new unsigned char[img_size];

    if (channels == 1) {  // Grayscale case
        for (int i = 0; i < img_size; i++) {
            thresholded_image[i] = (image[i] < threshold) ? 0 : 255;
        }
    }
    else if (channels == 3 || channels == 4) {  // RGB case
        for (int i = 0; i < img_size; i += channels) {
            if (colorSpace == ColorSpace::HSV) {
                // Convert RGB to HSV
                HSV hsv = rgbToHsv(image[i], image[i + 1], image[i + 2]);
                // Threshold the V channel
                unsigned char v_thresholded = (hsv.v * 255 < threshold) ? 0 : 255;
                // Set all RGB channels to the thresholded value
                thresholded_image[i] = thresholded_image[i + 1] = thresholded_image[i + 2] = v_thresholded;
            }
            else if (colorSpace == ColorSpace::HSL) {
                // Convert RGB to HSL
                HSL hsl = rgbToHsl(image[i], image[i + 1], image[i + 2]);
                // Threshold the L channel
                unsigned char l_thresholded = (hsl.l * 255 < threshold) ? 0 : 255;
                // Set all RGB channels to the thresholded value
                thresholded_image[i] = thresholded_image[i + 1] = thresholded_image[i + 2] = l_thresholded;
            }

            // Copy the alpha channel for RGBA images
            if (channels == 4) {
                thresholded_image[i + 3] = image[i + 3];
            }
        }
    }

    return thresholded_image;
}

/**
 * Applies salt and pepper noise to an image.
 *
 * @param image Pointer to the image data.
 * @param width Width of the image.
 * @param height Height of the image.
 * @param channels Number of color channels in the image.
 * @param noisePercentage The percentage of the image pixels to be affected by noise.
 * @return Pointer to the noisy image data.
 */
unsigned char* ColourCorrection::saltAndPepperNoise(unsigned char* image, int width, int height, int channels, float noisePercentage) {
    int img_size = width * height * channels;
    unsigned char* noisy_image = new unsigned char[img_size];
    std::memcpy(noisy_image, image, img_size * sizeof(unsigned char));

    // Initialize random seed
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Apply noise based on the noisePercentage
    for (int i = 0; i < img_size; i += channels) {
        float randPercent = (std::rand() % 10000) / 10000.0f;
        if (randPercent < noisePercentage / 100.0f) {
            // Set the pixel to black or white randomly
            unsigned char noise_color = (std::rand() % 2 == 0) ? 0 : 255;

            // Apply the same noise color to all color channels
            for (int ch = 0; ch < channels; ++ch) {
                if (channels == 4 && ch == 3) {  // Preserve the alpha channel
                    continue;
                }
                noisy_image[i + ch] = noise_color;
            }
        }
    }

    return noisy_image;
}