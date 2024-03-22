/**
 * @file User_3d.h
 * @author acse-yw3523,edsml-lwk16, acse-ad2123, 
 *         edsml-hs1623, acse-xg1123, edsml-st2923,
 *         Group: selection sort.
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