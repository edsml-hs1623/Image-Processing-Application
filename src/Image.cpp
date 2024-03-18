//Image.cpp
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "Image.h"
#include <iostream>


Image::Image() : width(0), height(0), channels(0), data(nullptr) {}

Image::~Image() {
    freeImage();
}

bool Image::loadImage(const std::string& filename) {
    freeImage(); // Ensure any previously loaded image is freed
    data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    if (!data) {
        std::cerr << "Error loading image: " << stbi_failure_reason() << std::endl;
        return false;
    }
    return true;
}

bool Image::saveImage(const std::string& filename, const std::string& format) {
    int success = 0;
    if (format == "png") {
        success = stbi_write_png(filename.c_str(), width, height, channels, data, width * channels);
    } else if (format == "bmp") {
        success = stbi_write_bmp(filename.c_str(), width, height, channels, data);
    } else if (format == "jpg") {
        success = stbi_write_jpg(filename.c_str(), width, height, channels, data, 100); // 100 is quality
    } else {
        std::cerr << "Unsupported image format for saving: " << format << std::endl;
        return false;
    }
    if (!success) {
        std::cerr << "Failed to save image." << std::endl;
        return false;
    }
    return true;
}

int Image::getWidth() const {
    return width;
}

int Image::getHeight() const {
    return height;
}

int Image::getChannels() const {
    return channels;
}

unsigned char* Image::getData() const {
    return data;
}

void Image::freeImage() {
    if (data != nullptr) {
        stbi_image_free(data);
        data = nullptr;
    }
}
