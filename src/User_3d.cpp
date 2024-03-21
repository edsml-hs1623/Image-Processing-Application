#include "Volume.h"
#include "Projection.h"
#include "ThreeDFilter.h"
#include "Slice.h"
#include "User_3d.h"
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
// Sets the filter parameters based on user input
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
// Generates projections from the processed volume and saves them with detailed filenames
void User_3D::generateProjections(const Volume& processedVolume, const std::string& filterType, int kernelSize, std::vector<std::string>& projectionPaths) {
    std::vector<std::string> projectionTypes = {"mip", "minip", "aip"};
    for (const auto& type : projectionTypes) {
        std::string kernelSizeStr = (filterType != "NoFilter") ? "_" + filterType + "_" + std::to_string(kernelSize) + "x" + std::to_string(kernelSize) + "x" + std::to_string(kernelSize) : "_nofilter";
        std::string outputPath = outputDir + "/" + type + kernelSizeStr + ".png";
        if (type == "mip") Projection::mip(processedVolume, outputPath);
        else if (type == "minip") Projection::minip(processedVolume, outputPath);
        else if (type == "aip") Projection::aip(processedVolume, outputPath);
        std::cout << type << " projection generated and saved: " << outputPath << "\n";
        projectionPaths.push_back(outputPath);
    }
}

void User_3D::handleSliceGeneration(const Volume& processedVolume) {
    int sliceIndexXZ = -1, sliceIndexYZ = -1; // Initialize with -1 to enter the loop
    // Loop for XZ slice index
    while (sliceIndexXZ != 0) {
        std::cout << "Enter XZ slice index (0 to skip, range: 1 to " << originalVolume.getDepth() << "): ";
        std::cin >> sliceIndexXZ;
        if (sliceIndexXZ > 0 && sliceIndexXZ <= originalVolume.getDepth()) {
            std::string sliceXZPath = outputDir + "/slice_xz_y" + std::to_string(sliceIndexXZ) + ".png";
            Slice::sliceXZ(processedVolume, sliceIndexXZ - 1, sliceXZPath);
            std::cout << "Slice XZ generated and saved: " << sliceXZPath << "\n";
            break; // Exit the loop after successful operation
        } else if (sliceIndexXZ == 0) {
            break; // Exit the loop if user chooses to skip
        } else {
            std::cerr << "Invalid XZ slice index. Valid range is 1 to " << originalVolume.getDepth() << ". Please try again.\n";
        }
    }
    // Loop for YZ slice index
    while (sliceIndexYZ != 0) {
        std::cout << "Enter YZ slice index (0 to skip, range: 1 to " << originalVolume.getWidth() << "): ";
        std::cin >> sliceIndexYZ;
        if (sliceIndexYZ > 0 && sliceIndexYZ <= originalVolume.getWidth()) {
            std::string sliceYZPath = outputDir + "/slice_yz_x" + std::to_string(sliceIndexYZ) + ".png";
            Slice::sliceYZ(processedVolume, sliceIndexYZ - 1, sliceYZPath);
            std::cout << "Slice YZ generated and saved: " << sliceYZPath << "\n";
            break; // Exit the loop after successful operation
        } else if (sliceIndexYZ == 0) {
            break; // Exit the loop if user chooses to skip
        } else {
            std::cerr << "Invalid YZ slice index. Valid range is 1 to " << originalVolume.getWidth() << ". Please try again.\n";
        }
    }
}
// Handles user interaction for generating slabs from the volume
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
// Main execution function
void User_3D::run() {
    int kernelSize = 0, filterChoice = 0;
    float sigma = 2.0; // Default sigma value
    std::string filterType;
    setFilterParameters(filterChoice, kernelSize, sigma, filterType);
    Volume processedVolume = originalVolume; // Assuming this is a deep copy or a modifiable original
    applyFilter(processedVolume, filterChoice, kernelSize, sigma);
    std::vector<std::string> projectionPaths;
    generateProjections(processedVolume, filterType, kernelSize, projectionPaths);
    if (askUserFor2DFilterApplication()) {
        for (const auto& projectionPath : projectionPaths) {
            process2DFiltersOnProjection(projectionPath);
        }
    } else {
        std::cout << "Skipping 2D filter application. Projections saved to " << outputDir << std::endl;
    }
    handleSliceGeneration(processedVolume);
    handleSlabGeneration(processedVolume);
}
// Asks the user if they want to apply 2D filters on the generated projections
bool User_3D::askUserFor2DFilterApplication() {
    std::cout << "Would you like to apply 2D filters to the projections? (y/n): ";
    char response;
    std::cin >> response;
    return (response == 'y' || response == 'Y');
}

// Processes the application of 2D filters on a given projection image
void User_3D::process2DFiltersOnProjection(const std::string& projectionPath) {
    Image image;
    if (!image.loadImage(projectionPath)) {
        std::cerr << "Failed to load projection image: " << projectionPath << std::endl;
        return;
    }
    std::cout << "Choose filter for the projection:\n"
              << "1. Grayscale\n"
              << "2. Brightness Adjustment\n"
              << "3. Histogram Equalization\n"
              << "4. Threshold\n"
              << "5. Salt and Pepper Noise\n"
              << "6. Blur\n"
              << "7. Edge Detection\n"
              << "Enter choice (1-7): ";
    int filterChoice;
    std::cin >> filterChoice;
    fs::path multiFilterDir = "../MultiFilter";
    fs::create_directories(fs::path(multiFilterDir)); // Ensure the output directory exists
    // Process chosen filter
    switch (filterChoice) {
        case 1:// Grayscale
        case 2: // Brightness Adjustment
        case 3: // Histogram Equalization
        case 4: // Threshold
        case 5: // Salt and Pepper Noise
            applyColourCorrection(fs::path(multiFilterDir), projectionPath, image, filterChoice);
            break;
        case 6: // Blur
            applyBlur(fs::path(multiFilterDir), projectionPath, image);
            break;
        case 7: // Edge Detection
            applyEdgeDetection(fs::path(multiFilterDir), projectionPath, image);
            break;
        default:
            std::cerr << "Invalid choice. No filter applied." << std::endl;
            break;
    }
    std::cout << "Processed projection saved to: " << multiFilterDir << std::endl;
}
 