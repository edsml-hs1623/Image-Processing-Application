/**
 * @file ImageBlur.h
 *
 * @brief Declaration of the ImageBlur class for applying various blur effects to images.
 *
 * The ImageBlur class extends from the Filter abstract class and implements different
 * blurring techniques such as Median, Box, and Gaussian blur. Each blur type is implemented
 * as a separate method, and the class allows for easy extension to include additional blur types.
 * The class provides a unified interface to apply these blurring effects on images.
 *
 * Key Features:
 *   - Support for Median, Box, and Gaussian blurring methods.
 *   - Extendable for additional blur types.
 *   - Easy application to Image objects.
 *
 * @note This class relies on a compatible Image class for image manipulation. Ensure that the Image class
 *       is properly included and defined in your project.
 */

#ifndef IMAGEBLUR_H
#define IMAGEBLUR_H

#include "Filter.h"
#include "Image.h"
#include "ImageBlur.h"
#include <iostream>
#include <string>
#include <vector>


enum BlurType { Median, Box, Gaussian };

class ImageBlur : public Filter {
public:
    ImageBlur(BlurType type, int kernelSize);

    virtual ~ImageBlur();

    void apply(Image &image) override;

private:
    BlurType blurType;
    int kernelSize;

    void applyBoxBlur(Image &image);
    void applyMedianBlur(Image &image);
    void applyGaussianBlur(Image &image);
    unsigned char findMedian(std::vector<unsigned char>& values);
    void selectionSort(std::vector<unsigned char>& arr);

    // Define other blur methods as needed
};

#endif // IMAGEBLUR_H