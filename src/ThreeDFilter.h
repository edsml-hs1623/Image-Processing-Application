#ifndef THREEDFILTER_H
#define THREEDFILTER_H

#include "Volume.h"

class ThreeDFilter {
public:
    static void gaussianBlur(Volume& volume, int kernelSize, float sigma);

    static void medianBlur(Volume& volume, int kernelSize);

private:
    static float gaussian(float x, float y, float z, float sigma);
    static unsigned char median(std::vector<unsigned char>& values);
    static void selectionSort(std::vector<unsigned char>& arr);
};

#endif // THREEDFILTER_H