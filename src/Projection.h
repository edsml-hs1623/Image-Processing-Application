#ifndef PROJECTION_H
#define PROJECTION_H

#include "Volume.h"
#include <string>
#include <limits>
#include <algorithm>

class Projection {
public:
    //function declarations with optional parameters for slab selection.
    //minZ and maxZ parameters allow the user to specify a range for the projection.
    // default to values that include the entire volume.
    static void mip(const Volume& volume, const std::string& outputPath, int minZ = 1, int maxZ = std::numeric_limits<int>::max());
    static void minip(const Volume& volume, const std::string& outputPath, int minZ = 1, int maxZ = std::numeric_limits<int>::max());
    static void aip(const Volume& volume, const std::string& outputPath, int minZ = 1, int maxZ = std::numeric_limits<int>::max());
};

#endif // PROJECTION_H
