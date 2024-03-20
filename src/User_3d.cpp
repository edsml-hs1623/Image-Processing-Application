#include "Volume.h"
#include "Projection.h"
#include "ThreeDFilter.h"
#include "Slice.h"

#include "User_3D.h"

User_3D::User_3D(const std::string& baseDir, const std::string& datasetName) {
    std::string datasetDir = baseDir + "/" + datasetName;
    if (!originalVolume.loadVolume(datasetDir)) {
        std::cerr << "Failed to load volume for dataset: " << datasetName << std::endl;
        exit(-1);
    }
    outputDir = "../8-3D/" + datasetName; // Adjust the path as needed
}

void User_3D::setFilterParameters(int& filterChoice, int& kernelSize, std::string& filterType) {
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
}

void User_3D::applyFilter(Volume& processedVolume, int filterChoice, int kernelSize) {
    if (filterChoice == 1) {
        float sigma = 1.0; // Assuming a default sigma value
        ThreeDFilter::gaussianBlur(processedVolume, kernelSize, sigma);
        std::cout << "Gaussian filter applied with kernel size " << kernelSize << ".\n";
    } else if (filterChoice == 2) {
        ThreeDFilter::medianBlur(processedVolume, kernelSize);
        std::cout << "Median filter applied with kernel size " << kernelSize << ".\n";
    }
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
    int sliceIndex;
    while (true) { // Start an infinite loop that will break once a valid input is received
        std::cout << "Enter slice index (0 to skip, range: 1 to " << originalVolume.getDepth() << "): ";
        std::cin >> sliceIndex;

        // Check if the input is within the valid range or if the user chose to skip
        if (sliceIndex == 0) {
            std::cout << "Skipping slice generation.\n";
            break; // Exit the loop if the user chooses to skip
        } else if (sliceIndex > 0 && sliceIndex <= originalVolume.getDepth()) {
            std::string sliceXZPath = outputDir + "/sliceXZ_" + std::to_string(sliceIndex) + ".png";
            std::string sliceYZPath = outputDir + "/sliceYZ_" + std::to_string(sliceIndex) + ".png";
            Slice::sliceXZ(processedVolume, sliceIndex - 1, sliceXZPath); // Adjust index for 0-based indexing
            Slice::sliceYZ(processedVolume, sliceIndex - 1, sliceYZPath);

            std::cout << "Slice XZ generated and saved: " << sliceXZPath << "\n";
            std::cout << "Slice YZ generated and saved: " << sliceYZPath << "\n";
            break; // Exit the loop after successfully processing the input
        } else {
            // Inform the user of the valid range and prompt again
            std::cerr << "Invalid slice index. Please ensure it is within the range: 1 to " << originalVolume.getDepth() << ".\n";
        }
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
    setFilterParameters(filterChoice, kernelSize, filterType);

    Volume processedVolume = originalVolume;
    applyFilter(processedVolume, filterChoice, kernelSize);

    generateProjections(processedVolume, filterType);
    handleSliceGeneration(processedVolume);
    handleSlabGeneration(processedVolume);
}




