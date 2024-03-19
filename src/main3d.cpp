#include "Volume.h"
#include "Slice.h"
#include "Projection.h"
#include <iostream>
#include <filesystem>
#include <string>

int main() {
    namespace fs = std::__fs::filesystem;

    std::cout << "Choose a dataset:\n1. confuciusornis\n2. fracture\nEnter choice (1 or 2): ";
    int choice;
    std::cin >> choice;

    std::string datasetName;
    if (choice == 1) {
        datasetName = "confuciusornis";
    } else if (choice == 2) {
        datasetName = "fracture";
    } else {
        std::cerr << "Invalid choice. Exiting program." << std::endl;
        return 1;
    }

    std::string baseDir = "../Scans"; // Adjust based on your actual base directory
    std::string volumeDirectory = baseDir + "/" + datasetName;

    Volume volume;
    if (!volume.loadVolume(volumeDirectory)) {
        std::cerr << "Failed to load volume from directory: " << volumeDirectory << std::endl;
        return 1;
    }

    std::cout << "Volume dimensions are Width: " << volume.getWidth() << ", Height: " << volume.getHeight() << std::endl;

    std::string outputBaseDir = "8-3D/" + datasetName;
    if (!fs::exists(outputBaseDir)) {
        fs::create_directories(outputBaseDir); // Ensure the output directory exists
    }

    // Perform and save projections
    Projection::mip(volume, outputBaseDir + "/mip-nofilter.png");
    Projection::minip(volume, outputBaseDir + "/minip-nofilter.png");
    Projection::aip(volume, outputBaseDir + "/aip-nofilter.png");

    std::cout << "Projections generated successfully." << std::endl;

    return 0;
}


//    std::cout << "Enter a y-coordinate for the x-z slice (0 to " << volume.getHeight() - 1 << "): ";
//    int ySlice;
//    std::cin >> ySlice;
//
//    std::cout << "Enter an x-coordinate for the y-z slice (0 to " << volume.getWidth() - 1 << "): ";
//    int xSlice;
//    std::cin >> xSlice;
//
//    // Construct output directory
//    std::string outputDir = "8-3D/" + datasetName;
//    if (!fs::exists(outputDir)) {
//        fs::create_directories(outputDir); // Ensure the output directory exists
//    }
//
//    // Generate slices and save
//    std::string outputPathXZ = outputDir + "/slice_xz_y" + std::to_string(ySlice) + ".png";
//    Slice::sliceXZ(volume, ySlice, outputPathXZ);
//    std::cout << "Generated " << outputPathXZ << std::endl;
//
//    std::string outputPathYZ = outputDir + "/slice_yz_x" + std::to_string(xSlice) + ".png";
//    Slice::sliceYZ(volume, xSlice, outputPathYZ);
//    std::cout << "Generated " << outputPathYZ << std::endl;
//
//    std::cout << "Slices generated successfully." << std::endl;
