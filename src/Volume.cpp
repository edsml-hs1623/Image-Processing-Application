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

const std::vector<std::vector<unsigned char>>& Volume::getData() const {
    return data;
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
