/**
 * @file User_3D.cpp
 *
 * @brief Header file for user interactions and volume processing in the 3D user interface.
 *
 * This header file declares the User_3D class, which manages user interactions and applies various
 * volume processing techniques such as filtering (Gaussian, Median), generating projections (MIP, MinIP, AIP),
 * and creating slices or slabs from the volume data. The class provides an interface for selecting datasets,
 * setting filter parameters, applying these settings to process the volume data, and guiding users through
 * the generation of different types of projections. Users have the option to apply 2D filters on these projections.
 * It uses functionalities from associated classes like Volume, Projection, and ThreeDFilter to perform its tasks.
 *
 * Key Functionalities:
 *   - Interface for selecting and loading datasets to process.
 *   - Setting parameters for volume filtering and applying the filters.
 *   - Generating Maximum Intensity Projection (MIP), Minimum Intensity Projection (MinIP), and
 *     Average Intensity Projection (AIP) from the processed volume.
 *   - Generating slices and slabs from specific indices within the volume with options for default or custom values.
 *   - Optional application of 2D filters on generated projections.
 *   - User interaction guides through the process and handles saving results to specified output directories.
 *
 * @note The class relies on a proper setup and inclusion of associated classes for volume handling and image processing,
 *       and it utilizes the standard filesystem library for directory and file management.
 *
 * Dependencies:
 *   - Volume.h
 *   - Projection.h
 *   - ThreeDFilter.h
 *   - Slice.h
 *   - <filesystem>, <iostream>, <vector>, <string>
 */
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

/**
 * @brief Constructs a User_3D object, prompting the user to select a dataset and loads the corresponding volume data.
 */
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

/**
 * @brief Interactively prompts the user to select a dataset for processing.
 *
 * Displays available datasets and reads user input to set the dataset name.
 */
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

/**
 * @brief Sets the filter parameters interactively based on user input.
 *
 * Asks the user to choose the filter type and parameters for the Gaussian or Median filter.
 *
 * @param filterChoice Reference to store the user's filter choice.
 * @param kernelSize Reference to store the chosen kernel size.
 * @param sigma Reference to store the chosen sigma value for Gaussian blur.
 * @param filterType Reference to store the filter type as a string.
 */
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

/**
 * @brief Applies the selected filter to the volume data.
 *
 * Uses the chosen parameters to apply either a Gaussian or Median filter to the volume.
 *
 * @param processedVolume A reference to the volume to which the filter will be applied.
 * @param filterChoice The filter choice made by the user.
 * @param kernelSize The size of the kernel to use for the filter.
 * @param sigma The sigma value for the Gaussian blur, if applicable.
 */
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

/**
 * @brief Generates and saves projection images from the processed volume data.
 *
 * Creates MIP, MinIP, and AIP projections as appropriate for the dataset and saves them to files.
 *
 * @param processedVolume The volume from which to generate projections.
 * @param filterType The type of filter applied, affecting the filenames of saved images.
 * @param kernelSize The size of the kernel used for filtering, affecting the filenames of saved images.
 */
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

/**
 * @brief Handles the generation of slice images based on default or user-provided indices.
 *
 * Interactively asks the user if default indices should be used or prompts for custom indices to generate slice images.
 *
 * @param processedVolume The volume from which to generate slice images.
 */
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

/**
 * @brief Generates a slice image from the processed volume data.
 *
 * Creates a slice image from the volume along the specified plane at the given index.
 *
 * @param processedVolume The volume from which to extract the slice.
 * @param index The index along the specified plane at which to take the slice.
 * @param plane The plane on which to take the slice ("xz" or "yz").
 */
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

/**
 * @brief Handles the generation of slab images based on default or user-provided indices.
 *
 * Interactively asks the user if default slab ranges should be used or prompts for custom ranges to generate slab projections.
 *
 * @param processedVolume The volume from which to generate slab projections.
 */
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

/**
 * @brief Generates slab projections using default indices specific to the dataset.
 *
 * Uses pre-defined start and end indices to create slab projections for the "confuciusornis" or "fracture" datasets.
 *
 * @param processedVolume The volume from which to generate the slab projections.
 */
void User_3D::generateDefaultSlabs(const Volume& processedVolume) {
    if (datasetName == "confuciusornis") {
        generateCustomSlab(processedVolume, 10, 70);
    } else if (datasetName == "fracture") {
        generateCustomSlab(processedVolume, 276, 476);
    }
}


/**
 * @brief Generates slab projections using user-provided start and end indices.
 *
 * Creates an AIP and MIP slab projection image from the given start to end indices within the volume.
 *
 * @param processedVolume The volume from which to generate the slab projections.
 * @param start The starting index for the slab projection.
 * @param end The ending index for the slab projection.
 */
void User_3D::generateCustomSlab(const Volume& processedVolume, int start, int end) {
    std::string aipPath = outputDir + "/aip-nofilter_thinslab_" + std::to_string(start) + "_" + std::to_string(end) + ".png";
    std::string mipPath = outputDir + "/mip-nofilter_thinslab_" + std::to_string(start) + "_" + std::to_string(end) + ".png";
    Projection::aip(processedVolume, aipPath, start, end);
    Projection::mip(processedVolume, mipPath, start, end);
    std::cout << "Custom slab projections generated and saved: " << aipPath << " and " << mipPath << "\n";
}

/**
 * @brief The main execution function that orchestrates the user interaction and processes the volume data.
 *
 * Guides the user through setting filter parameters, applying filters, and generating projections, slices, and slabs.
 */
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