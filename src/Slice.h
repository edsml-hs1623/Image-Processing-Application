// Slice.h
#ifndef SLICE_H
#define SLICE_H

#include "Volume.h"
#include <string>

class Slice {
public:
    static void sliceXZ(const Volume& volume, int y, const std::string& outputPath);
    static void sliceYZ(const Volume& volume, int x, const std::string& outputPath);
};

#endif // SLICE_H