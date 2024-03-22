/**
 * @file ColourCorrection.h
 *
 * @brief Defines the ColourCorrection class and supporting structures for color correction in images.
 *
 * This header file declares the ColourCorrection class and relevant data structures like HSL and HSV.
 * It provides an interface for applying various color correction techniques such as grayscale conversion,
 * brightness adjustment, histogram equalization, thresholding, and salt and pepper noise addition.
 * Additionally, it includes utility functions for color space conversions between RGB, HSV, and HSL.
 *
 * Key Structures:
 *   - HSL: Represents the Hue, Saturation, Lightness color model.
 *   - HSV: Represents the Hue, Saturation, Value color model.
 *   - ColorSpace: Enumerates the color spaces (HSV, HSL) used for specific correction operations.
 *   - ColourCorrectionType: Enumerates the types of color corrections available.
 *
 * Key Features:
 *   - Supports multiple color correction techniques for image processing.
 *   - Includes color space conversion utilities.
 *   - Extends from the Filter class, allowing integration into broader image processing pipelines.
 *
 * @note Ensure the Image class is compatible and that the necessary supporting structures are included.
 * @author acse-yw3523,edsml-lwk16, acse-ad2123, 
 *         edsml-hs1623, acse-xg1123, edsml-st2923,
 *         Group: selection sort.
 */

#ifndef COLOURCORRECTION_H
#define COLOURCORRECTION_H

#include "Filter.h"
#include "Image.h"


struct HSL {
    float h, s, l;
};

struct HSV {
    float h, s, v;
};

// Add an enum to help choose the color space for equalisation and thresholding
enum class ColorSpace {
    HSV,
    HSL
};

// Enum defining the types of colour correction that can be applied.
enum ColourCorrectionType { Grayscale, BrightnessAdjust, HistogramEqualization, Thresholding, SaltPepperNoise };

HSV rgbToHsv(unsigned char r, unsigned char g, unsigned char b);
void hsvToRgb(HSV hsv, unsigned char& r, unsigned char& g, unsigned char& b);
HSL rgbToHsl(unsigned char r, unsigned char g, unsigned char b);
void hslToRgb(HSL hsl, unsigned char& r, unsigned char& g, unsigned char& b);

class ColourCorrection : public Filter {
public:
    // Constructor that takes a colour correction type and an optional parameter.
    // The parameter can be used for operations like brightness adjustment or thresholding.
    ColourCorrection(ColourCorrectionType type, int param = 0, ColorSpace colorSpace = ColorSpace::HSV);

    // Virtual destructor to support proper cleanup in derived classes.
    virtual ~ColourCorrection();

    // Override the apply method from the Filter class to apply the selected colour correction.
    void apply(Image& image) override;

private:
    ColourCorrectionType correctionType; // Stores the selected type of colour correction.
    int parameter;  // Parameter for the correction, e.g., brightness value or threshold.
    ColorSpace colorSpace;
    
    static unsigned char* applyGrayscale(unsigned char* image, int width, int height, int channels);
    static unsigned char* adjustBrightness(unsigned char* image, int width, int height, int channels, int brightness);
    static unsigned char* histogramEqualisation(unsigned char* image, int width, int height, int channels, ColorSpace colorSpace);
    static unsigned char* applyThresholding(unsigned char* image, int width, int height, int channels, unsigned char threshold, ColorSpace colorSpace);
    static unsigned char* saltAndPepperNoise(unsigned char* image, int width, int height, int channels, float noisePercentage);
};

#endif // COLOURCORRECTION_H