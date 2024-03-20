/**
 * @file Image.h
 *
 * @brief Declaration of the Image class for managing and processing images.
 *
 * The Image class encapsulates functionalities for loading, saving, and handling image data.
 * It provides an abstraction over basic image manipulations and interactions, allowing for
 * ease of image processing in various formats. The class interfaces with the stb_image and
 * stb_image_write libraries for handling image input/output operations.
 *
 * Key Features:
 *   - Load images from file in various formats.
 *   - Save images to file in popular formats like PNG, BMP, and JPG.
 *   - Access and modify image properties such as width, height, and number of color channels.
 *   - Manipulate raw image data for advanced image processing tasks.
 *
 * @note This class assumes the presence of stb_image.h and stb_image_write.h in the project for handling image I/O.
 */

#ifndef IMAGE_H // Start of the include guard
#define IMAGE_H

#include <string>


class Image {
public:
    Image();
    ~Image();
    bool loadImage(const std::string& filename);
    bool saveImage(const std::string& filename, const std::string& format);
    int getWidth() const;
    int getHeight() const;
    int getChannels() const;
    unsigned char* getData() const;

    void updateData(unsigned char* newData, int newWidth, int newHeight, int newChannels);

private:
    int width, height, channels;
    unsigned char* data;

    void freeImage();
};


#endif // IMAGE_H

