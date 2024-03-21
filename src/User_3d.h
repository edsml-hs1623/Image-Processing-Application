/**
 * @file User_3D.h
 *
 * @brief Header file for user interactions and volume processing in the 3D user interface.
 *
 * This header file declares the User_3D class, which manages user interactions and applies various
 * volume processing techniques such as filtering (Gaussian, Median), generating projections (MIP, MinIP, AIP),
 * and creating slices or slabs from the volume data. The class provides an interface for selecting datasets,
 * setting filter parameters, and applying these settings to process the volume data. It also guides users through
 * generating different types of projections and allows for the application of 2D filters on these projections.
 * The class uses functionalities from associated classes like Volume, Projection, and ThreeDFilter to perform
 * its tasks.
 *
 * Key Functionalities:
 *   - Interface for selecting and loading datasets to process.
 *   - Setting parameters for volume filtering and applying the filters.
 *   - Generating Maximum Intensity Projection (MIP), Minimum Intensity Projection (MinIP), and
 *     Average Intensity Projection (AIP) from the processed volume.
 *   - Generating slices and slabs from specific indices within the volume.
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
    void run();
private:
    static constexpr char baseDir[] = "../Scans";
    std::string datasetName;
    std::string outputDir;
    Volume originalVolume;
    void selectDataset();
    void setFilterParameters(int& filterChoice, int& kernelSize, float& sigma, std::string& filterType);
    void applyFilter(Volume& processedVolume, int filterChoice, int kernelSize, float sigma);
    void generateProjections(const Volume& processedVolume, const std::string& filterType, int kernelSize, std::vector<std::string>& projectionPaths);
    void handleSliceGeneration(const Volume& processedVolume);
    void handleSlabGeneration(const Volume& processedVolume);
    bool askUserFor2DFilterApplication();
    void process2DFiltersOnProjection(const std::string& projectionPath);
};
#endif // USER_3D_H
 