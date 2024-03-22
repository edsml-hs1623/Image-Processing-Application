/**
 * @file Volume.cpp
 *
 * @brief Provides the implementation for the Volume class, handling 3D volume data manipulation.
 *
 * This file implements the Volume class, which encapsulates operations for loading, accessing, 
 * and saving 3D volume data represented as a series of 2D slices. The class provides functionalities 
 * to load volume data from disk, access volume properties and data, modify the volume data, and 
 * save the modified volume back to disk. It supports loading and saving volumes as a series of PNG images.
 *
 * Dependencies:
 *   - Volume.h for the declaration of the Volume class.
 *   - stb_image.h for loading PNG images as volume slices.
 *   - stb_image_write.h for saving volume slices as PNG images.
 *   - Standard libraries: <iostream>, <vector>, <algorithm>, <filesystem>, and <cstring>.
 */
#include "Volume.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <cstring> // for std::memcpy

namespace fs = std::filesystem;

/**
 * @brief Constructs a Volume object with initialized dimensions and channels.
 * Initializes a new Volume object with width, height, depth, and channels set to 0.
 */
Volume::Volume() : width(0), height(0), depth(0), channels(0) {}

/**
 * @brief Destructor for the Volume class.
 *
 * Cleans up the volume data by calling freeVolume() to release resources.
 */
Volume::~Volume() {
    freeVolume();
}

/**
 * @brief Gets the const reference to the volume data.
 *
 * Returns a constant reference to the internal data representing the volume.
 * @return A const reference to the data vector of the volume.
 */
const std::vector<std::vector<unsigned char>>& Volume::getData() const {
    return data;
}

/**
 * @brief Frees the memory allocated for the volume and resets its properties.
 *
 * Clears the volume data and sets the width, height, depth, and channels to 0.
 */
void Volume::freeVolume() {
    data.clear();
    width = 0;
    height = 0;
    depth = 0;
    channels = 0;
}

/**
 * @brief Loads volume data from a series of image slices in a directory.
 *
 * Iterates over PNG images in the given directory, loads them as slices of the volume,
 * and checks for consistent dimensions and channel count across slices.
 * @param directoryPath The filesystem path to the directory containing image slices.
 * @return True if the volume is loaded successfully, false otherwise.
 */
bool Volume::loadVolume(const std::string& directoryPath) {
    freeVolume();
    std::vector<std::string> fileNames;
    for (const auto& entry : fs::directory_iterator(directoryPath)) {
        if (entry.path().extension() == ".png") {
            fileNames.push_back(entry.path().string());
        }
    }
    std::sort(fileNames.begin(), fileNames.end());
    for (const auto& fileName : fileNames) {
        int w, h, ch;
        unsigned char* sliceData = stbi_load(fileName.c_str(), &w, &h, &ch, 0);
        if (!sliceData) {
            std::cerr << "Error loading slice: " << stbi_failure_reason() << std::endl;
            return false;
        }
        if (width == 0 && height == 0) {
            width = w;
            height = h;
            channels = ch;
        } else if (w != width || h != height || ch != channels) {
            std::cerr << "Error: Slice dimensions or channel count do not match." << std::endl;
            stbi_image_free(sliceData);
            return false;
        }
        data.emplace_back(sliceData, sliceData + (w * h * ch));
        stbi_image_free(sliceData);
    }
    depth = data.size();
    return true;
}


/**
 * @brief Gets the width of the volume.
 *
 * @return The width of the volume in voxels.
 */
int Volume::getWidth() const {
    return width;
}

/**
 * @brief Gets the height of the volume.
 *
 * @return The height of the volume in voxels.
 */
int Volume::getHeight() const {
    return height;
}

int Volume::getDepth() const {
    return depth;
}

/**
 * @brief Gets the depth of the volume.
 *
 * @return The depth of the volume in slices.
 */
int Volume::getChannels() const {
    return channels;
}

/**
 * @brief Gets the number of channels of the volume.
 *
 * @return The number of channels in the volume data.
 */
void Volume::setData(const std::vector<std::vector<unsigned char>>& newData) {
    data = newData;
    depth = data.size();
}

/**
 * @brief Sets the volume data to the new provided data.
 *
 * Assigns the new data to the volume and updates the depth accordingly.
 * @param newData A vector of new data to replace the current volume data.
 */
bool Volume::saveVolume(const std::string& directoryPath) const {
    fs::path dirPath(directoryPath);
    if (!fs::exists(dirPath)) {
        try {
            fs::create_directories(dirPath);
        } catch (const std::exception& e) {
            std::cerr << "Exception caught while creating directory: " << e.what() << std::endl;
            return false;
        }
    }

    for (size_t i = 0; i < data.size(); ++i) {
        std::string filePath = directoryPath + "/slice_" + std::to_string(i) + ".png";
        if (!stbi_write_png(filePath.c_str(), width, height, channels, data[i].data(), width * channels)) {
            std::cerr << "Failed to save slice " << i << std::endl;
            return false;
        }
    }
    return true;
}