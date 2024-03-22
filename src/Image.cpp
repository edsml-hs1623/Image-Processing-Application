/**
 * @file Image.cpp
 *
 * @brief Implementation of the Image class for handling image operations.
 *
 * This file includes the implementation of the Image class, which provides functionalities
 * for loading, saving, and manipulating image data. It uses the stb_image library for handling
 * various image formats and stb_image_write for saving images in different formats.
 * The class offers methods for loading images from files, saving images to files,
 * and accessing image properties like width, height, and number of color channels.
 *
 * Key Features:
 *   - Load images from files with support for multiple formats.
 *   - Save images to files in PNG, BMP, and JPG formats.
 *   - Access and manipulate basic image properties.
 *   - Interact with raw image data for advanced processing.
 *
 * Dependencies:
 *   - stb_image.h: For image loading capabilities.
 *   - stb_image_write.h: For image saving capabilities.
 *
 * Usage:
 *   Image img;
 *   img.loadImage("path/to/image.png");
 *   // Process img...
 *   img.saveImage("path/to/output.png", "png");
 *
 * @note This class relies on the stb_image and stb_image_write libraries, which should be included in the project.
 */

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#ifdef _MSC_VER
#define __STDC_LIB_EXT1__ 1
#endif
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "Image.h"
#include <iostream>


/**
* Constructor: Default constructor for the Image class.
* Initializes an empty image with zero width, height, and channels, and a null data pointer.
*/
Image::Image() : width(0), height(0), channels(0), data(nullptr) {}

/**
 * Destructor: Destructor for the Image class.
 * Ensures that any allocated image data is properly freed upon destruction of the object.
 */
Image::~Image() {
    freeImage();
}

/**
 * Loads an image from a file.
 *
 * @param filename The path to the image file to be loaded.
 * @return True if the image is loaded successfully; false otherwise.
 */
bool Image::loadImage(const std::string& filename) {
    freeImage(); // Ensure any previously loaded image is freed
    data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    if (!data) {
        std::cerr << "Error loading image: " << stbi_failure_reason() << std::endl;
        return false;
    }
    return true;
}

/**
 * Saves the current image to a file.
 *
 * @param filename The path where the image should be saved.
 * @param format The format to save the image in ("png", "bmp", or "jpg").
 * @return True if the image is saved successfully; false otherwise.
 */
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

/**
 * Gets the width of the image.
 *
 * @return The width of the image in pixels.
 */
int Image::getWidth() const {
    return width;
}

/**
 * Gets the height of the image.
 *
 * @return The height of the image in pixels.
 */
int Image::getHeight() const {
    return height;
}

/**
 * Gets the number of color channels in the image.
 *
 * @return The number of color channels.
 */
int Image::getChannels() const {
    return channels;
}

/**
 * Gets a pointer to the image data.
 *
 * @return A pointer to the image data, or nullptr if no data is loaded.
 */
unsigned char* Image::getData() const {
    return data;
}

/**
 * Frees the memory allocated for the image data.
 * Sets the data pointer to nullptr after freeing.
 */
void Image::freeImage() {
    if (data != nullptr) {
        stbi_image_free(data);
        data = nullptr;
    }
}

/**
 * Updates the image data with new data.
 *
 * @param newData Pointer to the new image data.
 * @param newWidth New width of the image.
 * @param newHeight New height of the image.
 * @param newChannels New number of color channels in the image.
 */
void Image::updateData(unsigned char* newData, int newWidth, int newHeight, int newChannels) {
    freeImage(); // Free existing data
    data = newData; // Update data pointer
    width = newWidth;
    height = newHeight;
    channels = newChannels;
}
