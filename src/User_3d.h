#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

class User_3D {
private:
    Volume originalVolume;
    std::string outputDir;
    void setFilterParameters(int& filterChoice, int& kernelSize, std::string& filterType);
    void applyFilter(Volume& processedVolume, int filterChoice, int kernelSize);
    void generateProjections(const Volume& processedVolume, const std::string& filterType);
    void handleSliceGeneration(const Volume& processedVolume);
    void handleSlabGeneration(const Volume& processedVolume);

public:
    User_3D(const std::string& baseDir, const std::string& datasetName);
    void run();
};