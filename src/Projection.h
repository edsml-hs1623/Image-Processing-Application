#ifndef PROJECTION_H
#define PROJECTION_H

#include "Volume.h"
#include <string>
#include <limits>
#include <algorithm>

class Projection {
public:
    static void mip(const Volume& volume, const std::string& outputPath); // Maximum Intensity Projection
    static void minip(const Volume& volume, const std::string& outputPath); // Minimum Intensity Projection
    static void aip(const Volume& volume, const std::string& outputPath); // Average Intensity Projection
    // Additional challenge: Implement median AIP
};

#endif // PROJECTION_H

