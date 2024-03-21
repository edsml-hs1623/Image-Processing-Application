#include "Volume.h"
#include "Projection.h"
#include "ThreeDFilter.h"
#include "Slice.h"
#include "User_3d.h"
#include <chrono>

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

User_3D::User_3D() {
    selectDataset();

    std::string datasetDir = std::string(baseDir) + "/" + datasetName;
    if (!originalVolume.loadVolume(datasetDir)) {
        std::cerr << "Failed to load volume for dataset: " << datasetName << std::endl;
        exit(-1);
    }

    outputDir = "/Users/yifeidou/advanced-programming-group-selection-sort/Output/3D/" + datasetName; // Adjust the path as needed
}

void User_3D::selectDataset() {
    std::cout << "Select the dataset you want to process:\n";
    std::cout << "1. Fracture\n";
    std::cout << "2. Confuciusornis\n";
    std::cout << "Enter your choice (1 or 2): ";

    int choice;
    std::cin >> choice;

    switch (choice) {
        case 1:
            datasetName = "fracture";
            break;
        case 2:
            datasetName = "confuciusornis";
            break;
        default:
            std::cerr << "Invalid choice. Exiting program.\n";
            exit(-1);
    }
}


void User_3D::setFilterParameters(int& filterChoice, int& kernelSize, float& sigma, std::string& filterType) {
    std::cout << "Volume dimensions: " << originalVolume.getWidth() << " (W) x "
              << originalVolume.getHeight() << " (H) x " << originalVolume.getDepth() << " (D)\n";
    std::cout << "Choose filter: 1 for Gaussian, 2 for Median, 0 for no filter: ";
    std::cin >> filterChoice;

    if (filterChoice != 0) {
        std::cout << "Enter kernel size (e.g., 3 for 3x3x3): ";
        std::cin >> kernelSize;
        filterType = (filterChoice == 1) ? "Gaussian" : "Median";

        if (filterChoice == 1) {
            std::cout << "Enter sigma value (default is 2.0): ";
            if (!(std::cin >> sigma)) { // If a non-numeric value is entered, revert to default
                std::cin.clear(); // Clear the error flag
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Skip bad input
                sigma = 2.0; // Default sigma
            }
        }

    } else {
        filterType = "NoFilter";
    }
}


void User_3D::applyFilter(Volume& processedVolume, int filterChoice, int kernelSize, float sigma) {
    using namespace std::chrono;
    auto start = high_resolution_clock::now();

    if (filterChoice == 1) {
        ThreeDFilter::gaussianBlur(processedVolume, kernelSize, sigma);
        std::cout << "Gaussian filter applied with kernel size " << kernelSize << " and sigma " << sigma << ".\n";
    } else if (filterChoice == 2) {
        ThreeDFilter::medianBlur(processedVolume, kernelSize);
        std::cout << "Median filter applied with kernel size " << kernelSize << ".\n";
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(end - start).count(); // Convert to seconds
    std::cout << "Filter application took " << duration << " seconds.\n";
}



void User_3D::generateProjections(const Volume& processedVolume, const std::string& filterType) {
    std::vector<std::string> projectionTypes = {"mip", "minip", "aip"};
    for (const auto& type : projectionTypes) {
        std::string outputPath = outputDir + "/" + type + "_" + filterType + ".png";
        if (type == "mip") Projection::mip(processedVolume, outputPath);
        else if (type == "minip") Projection::minip(processedVolume, outputPath);
        else if (type == "aip") Projection::aip(processedVolume, outputPath);

        std::cout << type << " projection generated and saved: " << outputPath << "\n";
    }
}

void User_3D::handleSliceGeneration(const Volume& processedVolume) {
    int sliceIndexXZ, sliceIndexYZ;

    // Prompt for XZ slice index
    std::cout << "Enter XZ slice index (0 to skip, range: 1 to " << originalVolume.getDepth() << "): ";
    std::cin >> sliceIndexXZ;
    if (sliceIndexXZ > 0 && sliceIndexXZ <= originalVolume.getDepth()) {
        std::string sliceXZPath = outputDir + "/sliceXZ_" + std::to_string(sliceIndexXZ) + ".png"; // For XZ slice
        Slice::sliceXZ(processedVolume, sliceIndexXZ - 1, sliceXZPath);
        std::cout << "Slice XZ generated and saved: " << sliceXZPath << "\n";
    } else if (sliceIndexXZ != 0) {
        std::cerr << "Invalid XZ slice index. Valid range is 1 to " << originalVolume.getDepth() << ".\n";
    }


    // Prompt for YZ slice index
    std::cout << "Enter YZ slice index (0 to skip, range: 1 to " << originalVolume.getWidth() << "): ";
    std::cin >> sliceIndexYZ;
    if (sliceIndexYZ > 0 && sliceIndexYZ <= originalVolume.getWidth()) {
        std::string sliceYZPath = outputDir + "/sliceYZ_" + std::to_string(sliceIndexYZ) + ".png"; // For YZ slice
        Slice::sliceYZ(processedVolume, sliceIndexYZ - 1, sliceYZPath);
        std::cout << "Slice YZ generated and saved: " << sliceYZPath << "\n";
    } else if (sliceIndexYZ != 0) {
        std::cerr << "Invalid YZ slice index. Valid range is 1 to " << originalVolume.getWidth() << ".\n";
    }
}


void User_3D::handleSlabGeneration(const Volume& processedVolume) {
    int slabStartIndex, slabWidth;
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
        Projection::minip(processedVolume, minipSlabPath, slabStartIndex, slabStartIndex + slabWidth - 1);

        std::cout << "MIP slab projection generated and saved: " << mipSlabPath << "\n";
        std::cout << "AIP slab projection generated and saved: " << aipSlabPath << "\n";
        std::cout << "MinIP slab projection generated and saved:" << minipSlabPath << "\n";
    } else {
        std::cerr << "Slab generation skipped or invalid input provided.\n";
    }
}

void User_3D::run() {
    int kernelSize, filterChoice;

    std::string filterType;
    float sigma = 2.0; // Default value or prompt the user for it

    setFilterParameters(filterChoice, kernelSize, sigma, filterType);


    Volume processedVolume = originalVolume;
    applyFilter(processedVolume, filterChoice, kernelSize, sigma);

    generateProjections(processedVolume, filterType);
    handleSliceGeneration(processedVolume);
    handleSlabGeneration(processedVolume);
}