#include "Volume.h"
#include <iostream>

int main() {
    // Replace with the path to your directory containing the scan images
    std::string directoryPath = "/Users/st2923/Desktop/c++/group_project/Scans/fracture";

    Volume volume;

    // Load the volume data from the specified directory
    if (!volume.loadVolume(directoryPath)) {
        std::cerr << "Failed to load volume." << std::endl;
        return 1; // Return a non-zero value to indicate failure
    }

    std::cout << "Volume loaded successfully!" << std::endl;
    std::cout << "Volume dimensions: " << volume.getWidth() << " x " << volume.getHeight() << " x " << volume.getDepth() << std::endl;

    // Save out the middle slice of the volume as an image to verify loading is correct
    int middleSliceIndex = volume.getDepth() / 2;
    if (!volume.saveSlice("/Users/st2923/Desktop/c++/group_project/Scans/Volumes_Tests/middle_slice.png", middleSliceIndex, "png")) {
        std::cerr << "Failed to save slice." << std::endl;
        return 1; // Return a non-zero value to indicate failure
    }

    std::cout << "Middle slice saved successfully!" << std::endl;

    return 0; // Success
}
