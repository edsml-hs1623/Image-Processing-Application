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
