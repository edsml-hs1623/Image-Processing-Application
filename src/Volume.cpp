#include "Volume.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <cstring> // for std::memcpy

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

std::vector<unsigned char> Volume::getSliceData(int sliceIndex) const {
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

bool Volume::saveSlice(const std::string& filename, int sliceIndex, const std::string& format) {
    if (sliceIndex < 0 || sliceIndex >= depth) {
        std::cerr << "Slice index out of range." << std::endl;
        return false;
    }
    const auto& sliceData = getSliceData(sliceIndex);
    int success = 0;
    if (format == "png") {
        success = stbi_write_png(filename.c_str(), width, height, channels, sliceData.data(), width * channels);
    } else if (format == "bmp") {
        success = stbi_write_bmp(filename.c_str(), width, height, channels, sliceData.data());
    } else {
        std::cerr << "Unsupported format. Only 'png' and 'bmp' are supported." << std::endl;
        return false;
    }
    return success != 0;
}
