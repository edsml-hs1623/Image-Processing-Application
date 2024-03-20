#include "User_3D.h"
#include <iostream>
#include <filesystem>

void User_3D::run() {
    std::string datasetName, baseDir = "../Scans";
    chooseDataset(datasetName, baseDir);

    std::string volumeDirectory = baseDir + "/" + datasetName;
    Volume volume;
    if (!loadVolume(volumeDirectory, volume)) {
        std::cerr << "Failed to load volume. Exiting." << std::endl;
        return;
    }

    std::string outputBaseDir = "./3D_Output/" + datasetName;
    std::filesystem::create_directories(outputBaseDir);

    applyFiltersAndGenerateProjections(volume, outputBaseDir);
    generateSlices(volume, outputBaseDir);
}

void User_3D::chooseDataset(std::string& datasetName, std::string& baseDir) {
    // Implement dataset selection logic here
}

bool User_3D::loadVolume(const std::string& volumeDirectory, Volume& volume) {
    // Implement volume loading logic here
    return true; // Placeholder
}

void User_3D::applyFiltersAndGenerateProjections(const Volume& volume, const std::string& outputBaseDir) {
    // Example: Apply a 3x3x3 Gaussian filter and generate a MIP
    applyGaussianFilter(volume, outputBaseDir, 3, 1.0);
    // Add more filters and projections as required
}

void User_3D::applyGaussianFilter(const Volume& volume, const std::string& outputBaseDir, int kernelSize, double sigma) {
    // Implement Gaussian filter application and projection generation here
}

void User_3D::applyMedianFilter(const Volume& volume, const std::string& outputBaseDir, int kernelSize) {
    // Implement Median filter application and projection generation here
}

void User_3D::generateSlices(const Volume& volume, const std::string& outputBaseDir) {
    // Implement slice generation here
}

void User_3D::specifySlab(int& minZ, int& maxZ, const Volume& volume) {
    // Implement slab specification logic here
}
