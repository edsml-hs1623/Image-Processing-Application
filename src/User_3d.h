/**
 * @file User_3D.h
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

#ifndef USER_3D_H
#define USER_3D_H

#include "Volume.h"
#include <string>
#include <vector>

class User_3D {
public:
    User_3D(); // Constructor
    void run(); // Main method to run the process

private:
    static constexpr char baseDir[] = "../Scans"; // Base directory for scans
    std::string datasetName; // Selected dataset name
    std::string outputDir; // Directory for saving output
    Volume originalVolume; // Original volume data

    void selectDataset(); // Method to select the dataset
    void setFilterParameters(int& filterChoice, int& kernelSize, float& sigma, std::string& filterType); // Set filter parameters
    void applyFilter(Volume& processedVolume, int filterChoice, int kernelSize, float sigma); // Apply chosen filter
    void generateProjections(const Volume& processedVolume, const std::string& filterType, int kernelSize); // Generate projections
    void handleSliceGeneration(const Volume& processedVolume); // Generate slices with user choice for default or custom values
    void handleSlabGeneration(const Volume& processedVolume); // Generate slabs with user choice for default or custom values

    // New methods for enhanced functionality
    void generateSlice(const Volume& processedVolume, int index, const std::string& plane); // Generate a single slice
    void generateCustomSlab(const Volume& processedVolume, int start, int end); // Generate a custom slab
    void generateDefaultSlabs(const Volume& processedVolume); // Generate default slabs based on dataset
};

#endif // USER_3D_H