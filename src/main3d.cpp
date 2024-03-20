#include "Volume.h"
#include "Projection.h"
#include "ThreeDFilter.h"
#include <filesystem>
#include <iostream>
#include "Slice.h"

namespace fs = std::filesystem;

void handleUserRequests(const Volume& originalVolume, const std::string& outputDir) {
    int kernelSize, filterChoice, sliceIndex, slabStartIndex, slabWidth;
    std::string filterType;

    // Display volume dimensions
    std::cout << "Volume dimensions: " << originalVolume.getWidth() << " (W) x "
              << originalVolume.getHeight() << " (H) x " << originalVolume.getDepth() << " (D)\n";

    std::cout << "Choose filter: 1 for Gaussian, 2 for Median, 0 for no filter: ";
    std::cin >> filterChoice;

    if (filterChoice != 0) {
        std::cout << "Enter kernel size (e.g., 3 for 3x3x3): ";
        std::cin >> kernelSize;
        filterType = (filterChoice == 1) ? "Gaussian" : "Median";
    } else {
        filterType = "NoFilter";
    }

    Volume processedVolume = originalVolume;
    if (filterChoice == 1) {
        float sigma = 1.0; // Assuming a default sigma value
        ThreeDFilter::gaussianBlur(processedVolume, kernelSize, sigma);
        std::cout << "Gaussian filter applied with kernel size " << kernelSize << ".\n";
    } else if (filterChoice == 2) {
        ThreeDFilter::medianBlur(processedVolume, kernelSize);
        std::cout << "Median filter applied with kernel size " << kernelSize << ".\n";
    }

    // Apply and save projections
    std::vector<std::string> projectionTypes = {"mip", "minip", "aip"};
    for (const auto& type : projectionTypes) {
        std::string outputPath = outputDir + "/" + type + "_" + filterType + ".png";
        if (type == "mip") Projection::mip(processedVolume, outputPath);
        else if (type == "minip") Projection::minip(processedVolume, outputPath);
        else if (type == "aip") Projection::aip(processedVolume, outputPath);

        std::cout << type << " projection generated and saved: " << outputPath << "\n";
    }

    std::cout << "Enter slice index (0 to skip, range: 1 to " << originalVolume.getDepth() << "): ";
    std::cin >> sliceIndex;
    if (sliceIndex > 0) {
        // Adjust sliceIndex for 0-based indexing
        int adjustedSliceIndex = sliceIndex - 1; // Adjust for 0-based indexing
        if (adjustedSliceIndex >= 0 && adjustedSliceIndex < originalVolume.getHeight()) {
            std::string sliceXZPath = outputDir + "/sliceXZ_" + std::to_string(sliceIndex) + ".png";
            std::string sliceYZPath = outputDir + "/sliceYZ_" + std::to_string(sliceIndex) + ".png";
            Slice::sliceXZ(processedVolume, adjustedSliceIndex, sliceXZPath);
            Slice::sliceYZ(processedVolume, adjustedSliceIndex, sliceYZPath);

            std::cout << "Slice XZ generated and saved: " << sliceXZPath << "\n";
            std::cout << "Slice YZ generated and saved: " << sliceYZPath << "\n";
        } else {
            std::cerr << "Invalid slice index. Please ensure it is within the range: 1 to " << originalVolume.getHeight() << ".\n";
        }
    }
    // Handle slab projection
    std::cout << "Enter start index for slab (0 to skip, range: 1 to " << originalVolume.getDepth() - 1 << " for start): ";
    std::cin >> slabStartIndex;
    if (slabStartIndex > 0) {
        std::cout << "Enter slab width (max possible width from start index: " << originalVolume.getDepth() - slabStartIndex << "): ";
        std::cin >> slabWidth;
        std::string mipSlabPath = outputDir + "/mip_slab_" + std::to_string(slabStartIndex) + "_" + std::to_string(slabWidth) + ".png";
        std::string aipSlabPath = outputDir + "/aip_slab_" + std::to_string(slabStartIndex) + "_" + std::to_string(slabWidth) + ".png";
        std::string minipSlabPath = outputDir + "/minip_slab_" + std::to_string(slabStartIndex) + "_" + std::to_string(slabWidth) + ".png";

        Projection::mip(processedVolume, mipSlabPath, slabStartIndex, slabStartIndex + slabWidth - 1);
        Projection::aip(processedVolume, aipSlabPath, slabStartIndex, slabStartIndex + slabWidth - 1);
        Projection::minip(processedVolume, aipSlabPath, slabStartIndex, slabStartIndex + slabWidth - 1);

        std::cout << "MIP slab projection generated and saved: " << mipSlabPath << "\n";
        std::cout << "AIP slab projection generated and saved: " << aipSlabPath << "\n";
        std::cout << "MinIP slab projection generated and saved: " << minipSlabPath << "\n";
    }
}

int main() {
    std::string baseDir = "../Scans";
    std::string datasetName = "fracture"; // Placeholder for your dataset name
    std::string datasetDir = baseDir + "/" + datasetName;

    Volume volume;
    if (!volume.loadVolume(datasetDir)) {
        std::cerr << "Failed to load volume for dataset: " << datasetName << std::endl;
        return -1;
    }

    std::string outputDir = "../8-3D/" + datasetName; // Adjust the path as needed
    handleUserRequests(volume, outputDir);

    return 0;
}











