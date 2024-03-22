/**
 * @file ThreeDFilter.h
 * @author acse-yw3523,edsml-lwk16, acse-ad2123, 
 *         edsml-hs1623, acse-xg1123, edsml-st2923,
 *         Group: selection sort.
 */
#ifndef THREEDFILTER_H
#define THREEDFILTER_H

#include "Volume.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


class ThreeDFilter {
public:
    static void gaussianBlur(Volume& volume, int kernelSize, float sigma);

    static void medianBlur(Volume& volume, int kernelSize);

private:

    static float gaussian(float x, float y, float z, float sigma);
    static unsigned char median(std::vector<unsigned char>& values);
    static void selectionSort(std::vector<unsigned char>& arr);

    //helper functions that optimize the gaussian blur
    //static std::vector<float> precomputeGaussianKernel(int kernelSize, float sigma);
    // static void applyGaussianBlur1D(std::vector<unsigned char>& line, const std::vector<float>& kernel, int halfSize);
};

#endif // THREEDFILTER_H