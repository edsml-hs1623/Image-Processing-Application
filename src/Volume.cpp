#include "Volume.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <cstring> // for std::memcpy
#include <fstream>
#include <iomanip> // for std::setw and std::setfill
#include <sstream>

namespace fs = std::__fs::filesystem;

Volume::Volume() : width(0), height(0), depth(0), channels(0) {}

Volume::~Volume() {
    freeVolume();
}

void Volume::freeVolume() {
    data.clear();
    width = 0;
    height = 0;
    depth = 0;
    channels = 0;
}

bool Volume::loadVolume(const std::string& directoryPath) {
    freeVolume();
    std::vector<std::string> fileNames;
    for (const auto& entry : fs::directory_iterator(directoryPath)) {
        fileNames.push_back(entry.path().string());
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

std::vector<unsigned char> Volume::getData(int sliceIndex) const {
    if (sliceIndex < 0 || sliceIndex >= depth) {
        std::cerr << "Slice index out of bounds." << std::endl;
        return std::vector<unsigned char>(); // Return an empty vector to indicate error
    }
    return data[sliceIndex];
}

int Volume::getWidth() const {
    return width;
}

int Volume::getHeight() const {
    return height;
}

int Volume::getDepth() const {
    return depth;
}

int Volume::getChannels() const {
    return channels;
}

void Volume::setVoxelValue(int z, int y, int x, unsigned char newValue) {
    if (z < 0 || z >= depth || y < 0 || y >= height || x < 0 || x >= width) {
        std::cerr << "Voxel coordinates out of bounds." << std::endl;
        return;
    }
    // Assuming that each slice is a flat array in row-major order:
    data[z][y * width + x] = newValue;
}

// Method to save a single slice as an image
bool Volume::saveSliceAsImage(int sliceIndex, const std::string& filePath) const {
    if (sliceIndex < 0 || sliceIndex >= depth) {
        std::cerr << "Slice index out of bounds: " << sliceIndex << std::endl;
        return false;
    }

    const std::vector<unsigned char>& sliceData = data[sliceIndex];
    int stride_in_bytes = width * channels;

    if (!stbi_write_png(filePath.c_str(), width, height, channels, sliceData.data(), stride_in_bytes)) {
        std::cerr << "Failed to save image: " << filePath << std::endl;
        return false;
    }

    return true;
}

// Method to save all slices in the volume as images
bool Volume::saveVolumeAsImages(const std::string& outputDirectory) const {
    for (int i = 0; i < depth; ++i) {
        std::ostringstream oss;
        oss << outputDirectory << "/slice_" << std::setw(4) << std::setfill('0') << i << ".png";
        std::string filePath = oss.str();

        if (!saveSliceAsImage(i, filePath)) {
            std::cerr << "Failed to save slice " << i << " as an image." << std::endl;
            return false; // Stop if we fail to save a slice
        }
    }
    return true;
}