#include "Volume.h"
#include "Slice.h"
#include "Projection.h"
#include <iostream>
#include <filesystem>
#include <string>
#include "VolumeFilter.h"



int main() {
    namespace fs = std::__fs::filesystem;

    // Step 1: Choose the dataset
    std::string datasetName = "confuciusornis"; // For testing, directly assign the dataset name
    std::string baseDir = "../Scans";
    std::string volumeDirectory = baseDir + "/" + datasetName;

    // Step 2: Load the volume
    Volume volume;
    if (!volume.loadVolume(volumeDirectory)) {
        std::cerr << "Failed to load volume from directory: " << volumeDirectory << std::endl;
        return 1;
    }
    std::cout << "Volume loaded successfully.\n";

    // Define filter parameters (for testing, use a single kernel size and sigma)
    int kernelSize = 3;
    double sigma = 1.0;

    // Step 3: Apply the 3D Gaussian Filter
    Volume gaussianFilteredVolume = VolumeFilter::applyGaussianFilter(volume, kernelSize, sigma);
    std::cout << "Gaussian filter applied.\n";

    // Setup the output directory
    std::string outputBaseDir = "./3D_Output/" + datasetName;
    if (!fs::exists(outputBaseDir)) {
        fs::create_directories(outputBaseDir);
    }

    // Step 4: Generate Projections
    // Original volume
    Projection::mip(volume, outputBaseDir + "/mip-nofilter.png");
    std::cout << "MIP generated for original volume.\n";

    // Gaussian filtered volume
    Projection::mip(gaussianFilteredVolume, outputBaseDir + "/mip-gaussian_" + std::to_string(kernelSize) + "x" + std::to_string(kernelSize) + "x" + std::to_string(kernelSize) + ".png");
    std::cout << "MIP generated for Gaussian filtered volume.\n";

    std::cout << "All projections generated successfully.\n";

    return 0;
}





////main working for slice and projection
//int main() {
//    namespace fs = std::__fs::filesystem;
//
//    std::cout << "Choose a dataset:\n1. confuciusornis\n2. fracture\nEnter choice (1 or 2): ";
//    int choice;
//    std::cin >> choice;
//
//    std::string datasetName;
//    if (choice == 1) {
//        datasetName = "confuciusornis";
//    } else if (choice == 2) {
//        datasetName = "fracture";
//    } else {
//        std::cerr << "Invalid choice. Exiting program." << std::endl;
//        return 1;
//    }
//
//    std::string baseDir = "../Scans";
//    std::string volumeDirectory = baseDir + "/" + datasetName;
//
//    Volume volume;
//    if (!volume.loadVolume(volumeDirectory)) {
//        std::cerr << "Failed to load volume from directory: " << volumeDirectory << std::endl;
//        return 1;
//    }
//
//    std::cout << "Volume dimensions are Width: " << volume.getWidth() << ", Height: " << volume.getHeight() << ", Depth: " << volume.getDepth() << std::endl;
//
//    // Ask the user if they want to specify a slab
//    std::cout << "Would you like to specify a slab? (y/n): ";
//    char specifySlab;
//    std::cin >> specifySlab;
//
//    int minZ = 1; // Default to first slice
//    int maxZ = volume.getDepth(); // Default to last slice
//
//    if (specifySlab == 'y' || specifySlab == 'Y') {
//        std::cout << "Enter the minimum z-coordinate for the slab (1 to " << volume.getDepth() << "): ";
//        std::cin >> minZ;
//        std::cout << "Enter the maximum z-coordinate for the slab (1 to " << volume.getDepth() << "): ";
//        std::cin >> maxZ;
//    }
//
//    std::string outputBaseDir = "./3D_Output/" + datasetName;
//    if (!fs::exists(outputBaseDir)) {
//        fs::create_directories(outputBaseDir);
//    }
//
//    // Perform and save projections with the specified or default slab selection
//    Projection::mip(volume, outputBaseDir + "/mip-slab.png", minZ, maxZ);
//    Projection::minip(volume, outputBaseDir + "/minip-slab.png", minZ, maxZ);
//    Projection::aip(volume, outputBaseDir + "/aip-slab.png", minZ, maxZ);
//
//    std::cout << "Projections generated successfully." << std::endl;
//    // Slicing
//    std::cout << "Enter a y-coordinate for the x-z slice (1 to " << volume.getHeight() << "): ";
//    int ySlice;
//    std::cin >> ySlice;
//
//    std::cout << "Enter an x-coordinate for the y-z slice (1 to " << volume.getWidth() << "): ";
//    int xSlice;
//    std::cin >> xSlice;
//
//    // Generate slices and save
//    std::string outputPathXZ = outputBaseDir + "/slice_xz_y" + std::to_string(ySlice) + ".png";
//    Slice::sliceXZ(volume, ySlice, outputPathXZ);
//    std::cout << "Generated " << outputPathXZ << std::endl;
//
//    std::string outputPathYZ = outputBaseDir + "/slice_yz_x" + std::to_string(xSlice) + ".png";
//    Slice::sliceYZ(volume, xSlice, outputPathYZ);
//    std::cout << "Generated " << outputPathYZ << std::endl;
//
//    std::cout << "Slices generated successfully." << std::endl;
//
//    return 0;
//}
