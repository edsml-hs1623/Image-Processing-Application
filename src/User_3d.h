#ifndef USER_3D_H
#define USER_3D_H

#include "Volume.h"
#include "VolumeFilter.h"
#include "Projection.h"
#include "Slice.h"
#include <string>

class User_3D {
public:
    void run();

private:
    void chooseDataset(std::string& datasetName, std::string& baseDir);
    bool loadVolume(const std::string& volumeDirectory, Volume& volume);
    void applyFiltersAndGenerateProjections(const Volume& volume, const std::string& outputBaseDir);
    void applyGaussianFilter(const Volume& volume, const std::string& outputBaseDir, int kernelSize, double sigma);
    void applyMedianFilter(const Volume& volume, const std::string& outputBaseDir, int kernelSize);
    void generateSlices(const Volume& volume, const std::string& outputBaseDir);
    void specifySlab(int& minZ, int& maxZ, const Volume& volume);
};

#endif // USER_3D_H

