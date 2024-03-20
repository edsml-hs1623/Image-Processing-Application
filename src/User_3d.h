#ifndef USER_3D_H
#define USER_3D_H

#include "Volume.h"
#include "Projection.h"
#include "ThreeDFilter.h"
#include "Slice.h"
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

class User_3D {
public:
    User_3D(); // Constructor declaration changed
    void run();

private:

    static constexpr char baseDir[] = "../Scans"; // Static base directory path
    std::string datasetName;
    std::string outputDir;
    Volume originalVolume;

    void selectDataset();
    void setFilterParameters(int& filterChoice, int& kernelSize, std::string& filterType);
    void applyFilter(Volume& processedVolume, int filterChoice, int kernelSize);
    void generateProjections(const Volume& processedVolume, const std::string& filterType);
    void handleSliceGeneration(const Volume& processedVolume);
    void handleSlabGeneration(const Volume& processedVolume);
};

#endif // USER_3D_H