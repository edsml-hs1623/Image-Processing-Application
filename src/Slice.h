/**
 * @file Slice.h
 * @author acse-yw3523,edsml-lwk16, acse-ad2123, 
 *         edsml-hs1623, acse-xg1123, edsml-st2923,
 *         Group: selection sort.
 */
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