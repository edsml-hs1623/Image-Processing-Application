#include <iostream>
#include "Image.h"


int main() {
    Image img;

    // Load an image
    std::string inputFilename = "../Images/gracehopper.png";
    if (!img.loadImage(inputFilename)) {
        std::cerr << "Failed to load image: " << inputFilename << std::endl;
        return 1;
    }
    std::cout << "Loaded image: " << inputFilename << std::endl;
    std::cout << "Image dimensions: " << img.getWidth() << "x" << img.getHeight() << " - Channels: " << img.getChannels() << std::endl;

    // Example manipulation: Invert colors
    // Note: This is a simplistic example. For actual filter implementations, modify accordingly.
    unsigned char* data = img.getData();
    int width = img.getWidth();
    int height = img.getHeight();
    int channels = img.getChannels();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = (y * width + x) * channels;
            for (int c = 0; c < channels; ++c) { // Invert each channel except alpha
                if (c < 3) { // Assuming the alpha channel is the last one if exists
                    data[index + c] = 255 - data[index + c];
                }
            }
        }
    }

    // Save the manipulated image
    std::string outputFilename = "../Images/gracehopper_test.png";
    if (!img.saveImage(outputFilename, "png")) {
        std::cerr << "Failed to save image: " << outputFilename << std::endl;
        return 1;
    }
    std::cout << "Saved manipulated image to: " << outputFilename << std::endl;

    return 0;
}
