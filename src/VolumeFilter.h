#ifndef VOLUMEFILTER_H
#define VOLUMEFILTER_H

#include "Volume.h"
#include <vector>

class VolumeFilter {
public:
    static Volume applyGaussianFilter(const Volume& volume, int kernelSize, double sigma);
    static Volume applyMedianFilter(const Volume& volume, int kernelSize);

private:
    static std::vector<std::vector<std::vector<double>>> generateGaussianKernel(int kernelSize, double sigma);
    static unsigned char calculateMedianValue(std::vector<unsigned char>& values);
};

#endif // VOLUMEFILTER_H