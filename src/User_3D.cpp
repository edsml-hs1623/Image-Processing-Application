#include "Volume.h"
#include "Projection.h"
#include "ThreeDFilter.h"
#include "Slice.h"
#include "User_3D.h"
#include <chrono>
#include <iostream>
#include <filesystem>
#include <vector>
#include "User_2D.h"

namespace fs = std::filesystem;

// Constructor
User_3D::User_3D() {
    selectDataset();

    // Constructing dataset directory path and loading volume data
    std::string datasetDir = std::string(baseDir) + "/" + datasetName;
    if (!originalVolume.loadVolume(datasetDir)) {
        std::cerr << "Failed to load volume for dataset: " << datasetName << std::endl;
        exit(-1);
    }

    // Preparing the output directory for saving processed images
    outputDir = "../Output/8-3D/" + datasetName; // Adjust the path as needed
    fs::create_directories(fs::path(outputDir)); // Ensure the output directory exists
}

// Selects a dataset based on user input
void User_3D::selectDataset() {
    std::cout << "\nSelect the dataset you want to process:\n";
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

// Sets the filter parameters based on user input
void User_3D::setFilterParameters(int& filterChoice, int& kernelSize, float& sigma, std::string& filterType) {
    std::cout << "Volume dimensions: " << originalVolume.getWidth() << " (W) x "
              << originalVolume.getHeight() << " (H) x " << originalVolume.getDepth() << " (D)\n";
    std::cout << "\nChoose filter: 1 for Gaussian, 2 for Median, 0 for no filter: ";
    std::cin >> filterChoice;

    if (filterChoice != 0) {
        std::cout << "Enter kernel size (e.g., 3 for 3x3x3): ";
        std::cin >> kernelSize;
        filterType = (filterChoice == 1) ? "Gaussian" : "Median";

        if (filterChoice == 1) {
            std::cout << "Enter sigma value (default is 2.0): ";
            std::cin >> sigma; // Assuming validation and error handling for sigma
        }
    } else {
        filterType = "NoFilter";
    }
}

// Applies the chosen filter to the volume data
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


void User_3D::generateProjections(const Volume& processedVolume, const std::string& filterType, int kernelSize) {
    std::vector<std::string> projectionTypes;
    if (datasetName == "confuciusornis") {
        projectionTypes = {"mip", "aip"};
    } else if (datasetName == "fracture") {
        projectionTypes = {"mip", "minip", "aip"};
    }

    for (const auto& type : projectionTypes) {
        std::string kernelSizeStr = (filterType != "NoFilter") ? "_" + filterType + "_" + std::to_string(kernelSize) + "x" + std::to_string(kernelSize) + "x" + std::to_string(kernelSize) : "_nofilter";
        std::string outputPath = outputDir + "/" + type + kernelSizeStr + ".png";
        if (type == "mip") Projection::mip(processedVolume, outputPath);
        else if (type == "minip" && datasetName == "fracture") Projection::minip(processedVolume, outputPath);
        else if (type == "aip") Projection::aip(processedVolume, outputPath);

        std::cout << type << " projection generated and saved: " << outputPath << "\n";
    }
}

void User_3D::handleSliceGeneration(const Volume& processedVolume) {
    std::cout << "\nWould you like to use default slice indices? (y/n): ";
    char useDefaultSlices;
    std::cin >> useDefaultSlices;

    if (useDefaultSlices == 'y' || useDefaultSlices == 'Y') {
        if (datasetName == "confuciusornis") {
            // Use default indices for confuciusornis
            generateSlice(processedVolume, 420, "xz");
            generateSlice(processedVolume, 400, "yz");
        } else if (datasetName == "fracture") {
            // Use default indices for fracture
            generateSlice(processedVolume, 138, "xz");
            generateSlice(processedVolume, 275, "yz");
        }
    } else {
        // Ask user for custom indices
        int xzIndex, yzIndex;
        std::cout << "Enter custom XZ slice index: ";
        std::cin >> xzIndex;
        std::cout << "Enter custom YZ slice index: ";
        std::cin >> yzIndex;
        generateSlice(processedVolume, xzIndex, "xz");
        generateSlice(processedVolume, yzIndex, "yz");
    }
}

void User_3D::generateSlice(const Volume& processedVolume, int index, const std::string& plane) {
    std::string outputPath;

    if (plane == "xz") {
        outputPath = outputDir + "/slice_xz_y" + std::to_string(index) + ".png"; // xz slice uses y coordinate in name
        Slice::sliceXZ(processedVolume, index, outputPath);
    } else if (plane == "yz") {
        outputPath = outputDir + "/slice_yz_x" + std::to_string(index) + ".png"; // yz slice uses x coordinate in name
        Slice::sliceYZ(processedVolume, index, outputPath);
    }

    //std::cout << "Slice " << plane << " generated and saved: " << outputPath << "\n";
}


void User_3D::handleSlabGeneration(const Volume& processedVolume) {
    std::cout << "\nWould you like to use default slab indices? (y/n): ";
    char useDefaultSlabs;
    std::cin >> useDefaultSlabs;

    if (useDefaultSlabs == 'y' || useDefaultSlabs == 'Y') {
        generateDefaultSlabs(processedVolume);
    } else {
        // Ask user for custom slab parameters
        int startIndex, endIndex;
        std::cout << "Enter custom slab start index: ";
        std::cin >> startIndex;
        std::cout << "Enter custom slab end index: ";
        std::cin >> endIndex;
        generateCustomSlab(processedVolume, startIndex, endIndex);
    }
}

void User_3D::generateDefaultSlabs(const Volume& processedVolume) {
    if (datasetName == "confuciusornis") {
        generateCustomSlab(processedVolume, 10, 70);
    } else if (datasetName == "fracture") {
        generateCustomSlab(processedVolume, 276, 476);
    }
}

void User_3D::generateCustomSlab(const Volume& processedVolume, int start, int end) {
    std::string aipPath = outputDir + "/aip-nofilter_thinslab_" + std::to_string(start) + "_" + std::to_string(end) + ".png";
    std::string mipPath = outputDir + "/mip-nofilter_thinslab_" + std::to_string(start) + "_" + std::to_string(end) + ".png";
    Projection::aip(processedVolume, aipPath, start, end);
    Projection::mip(processedVolume, mipPath, start, end);
    std::cout << "Custom slab projections generated and saved: " << aipPath << " and " << mipPath << "\n";
}


// Main execution function
void User_3D::run() {
    // Setup and apply filters as before
    int kernelSize = 0, filterChoice = 0;
    float sigma = 2.0;
    std::string filterType;
    setFilterParameters(filterChoice, kernelSize, sigma, filterType);

    Volume processedVolume = originalVolume;
    applyFilter(processedVolume, filterChoice, kernelSize, sigma);

    // Now directly generate required projections, slices, and slabs
    generateProjections(processedVolume, filterType, kernelSize);
    handleSliceGeneration(processedVolume);
    handleSlabGeneration(processedVolume);

}