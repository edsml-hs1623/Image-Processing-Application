/**
 * @file EdgeDetection.cpp
 *
 * @brief Implementation of the EdgeDetection class for detecting edges in images.
 *
 * This file contains the implementation of the EdgeDetection class, which includes methods
 * to apply various edge detection operators like Sobel, Prewitt, Scharr, and Roberts Cross.
 * The class is designed to preprocess images using grayscale conversion and Gaussian blur
 * (except in certain cases like Roberts Cross), followed by the application of the chosen
 * edge detection operator. Each operator is implemented as a separate method, and a common
 * method is used to apply the edge detection logic.
 *
 * Key Features:
 *   - Supports various edge detection techniques.
 *   - Preprocesses images for optimal edge detection.
 *   - Provides flexibility to extend and add more edge detection methods.
 *
 * Usage:
 *   EdgeDetection edgeDetector(EdgeDetection::Sobel);
 *   edgeDetector.apply(image); // 'image' is an instance of Image class
 *
 * @note This class relies on other image processing classes like ColourCorrection and ImageBlur
 *       and expects them to be defined and included in the project.
 * @author acse-yw3523,edsml-lwk16, acse-ad2123, 
 *         edsml-hs1623, acse-xg1123, edsml-st2923,
 *         Group: selection sort.
 */

#include "EdgeDetection.h"
#include "ImageBlur.h"
#include "ColourCorrection.h"
#include <vector>
#include <algorithm> // for std::min and std::max
#include <filesystem>
#include <cmath>


/**
* Constructor: Constructor for EdgeDetection class.
*
* @param operatorType The type of edge operator to use (e.g., Sobel, Prewitt).
*/
EdgeDetection::EdgeDetection(EdgeOperator operatorType) : operatorType(operatorType) {}

/**
 * Destructor: Destructor for EdgeDetection class.
 */
EdgeDetection::~EdgeDetection() {}

/**
 * Applies the selected edge detection operator to an image.
 *
 * @param image The image to apply edge detection on.
 */
void EdgeDetection::apply(Image& image) {
    switch (operatorType) {
    case Sobel:
        applySobel(image);
        break;
    case Prewitt:
        applyPrewitt(image);
        break;
    case Scharr:
        applyScharr(image);
        break;
    case RobertsCross:
        applyRobertsCross(image);
        break;
    default:
        std::cerr << "Unknown edge operator." << std::endl;
        break;
    }
}

/**
 * Applies the Sobel edge detection operator to an image.
 * This involves grayscale conversion, Gaussian blur, and Sobel convolution.
 *
 * @param image The image to apply Sobel operator on.
 */
void EdgeDetection::applySobel(Image& image) {
    int width = image.getWidth();
    int height = image.getHeight();
    int channels = image.getChannels();
    unsigned char* data = image.getData();
    std::vector<unsigned char> newData(width * height * channels);

    // Apply grayscale as preprocessing step
    ColourCorrection grayscale(Grayscale);
    grayscale.apply(image);

    // Apply Gaussian blur as preprocessing step
    ImageBlur blur(Gaussian, 5); // Adjust kernel size as needed
    blur.apply(image);

    // Sobel operator kernels
    const std::vector<std::vector<int>> sobelKernelX = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
    const std::vector<std::vector<int>> sobelKernelY = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };

    // Apply Sobel operator
    applyEdgeDetection(image, sobelKernelX, sobelKernelY);
}

/**
 * Applies the Prewitt edge detection operator to an image.
 * This involves grayscale conversion, Gaussian blur, and Prewitt convolution.
 *
 * @param image The image to apply Prewitt operator on.
 */
void EdgeDetection::applyPrewitt(Image& image) {
    int width = image.getWidth();
    int height = image.getHeight();
    int channels = image.getChannels();
    unsigned char* data = image.getData();
    std::vector<unsigned char> newData(width * height * channels);

    // Apply grayscale as preprocessing step
    ColourCorrection grayscale(Grayscale);
    grayscale.apply(image);

    // Apply Gaussian blur as preprocessing step
    ImageBlur blur(Gaussian, 5); // Adjust kernel size as needed
    blur.apply(image);

    // Prewitt operator kernels
    const std::vector<std::vector<int>> prewittKernelX = { {-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1} };
    const std::vector<std::vector<int>> prewittKernelY = { {-1, -1, -1}, {0, 0, 0}, {1, 1, 1} };

    // Apply Prewitt operator
    applyEdgeDetection(image, prewittKernelX, prewittKernelY);
}

/**
 * Applies the Scharr edge detection operator to an image.
 * This involves grayscale conversion, Gaussian blur, and Scharr convolution.
 *
 * @param image The image to apply Scharr operator on.
 */
void EdgeDetection::applyScharr(Image& image) {
    int width = image.getWidth();
    int height = image.getHeight();
    int channels = image.getChannels();
    unsigned char* data = image.getData();
    std::vector<unsigned char> newData(width * height * channels);

    // Apply grayscale as preprocessing step
    ColourCorrection grayscale(Grayscale);
    grayscale.apply(image);

    // Apply Gaussian blur as preprocessing step
    ImageBlur blur(Gaussian, 5); // Adjust kernel size as needed
    blur.apply(image);

    // Scharr operator kernels
    const std::vector<std::vector<int>> scharrKernelX = { {-3, 0, 3}, {-10, 0, 10}, {-3, 0, 3} };
    const std::vector<std::vector<int>> scharrKernelY = { {-3, -10, -3}, {0, 0, 0}, {3, 10, 3} };

    // Apply Scharr operator
    applyEdgeDetection(image, scharrKernelX, scharrKernelY);
}

/**
 * Applies the Roberts Cross edge detection operator to an image.
 * This involves grayscale conversion and Roberts Cross convolution.
 * Note: Gaussian blur is not applied for Roberts Cross.
 *
 * @param image The image to apply Roberts Cross operator on.
 */
void EdgeDetection::applyRobertsCross(Image& image) {
    int width = image.getWidth();
    int height = image.getHeight();
    int channels = image.getChannels();
    unsigned char* data = image.getData();
    std::vector<unsigned char> newData(width * height * channels);

    // Apply grayscale as preprocessing step
    ColourCorrection grayscale(Grayscale);
    grayscale.apply(image);

    // /* Gaussian blur is not applied for Roberts Cross as no blur produced better results.
    // If the blur is applied too much noise is reduced; image appears very dark.*/
    // // Apply Gaussian blur as preprocessing step
    // ImageBlur blur(Gaussian, 5); // Adjust kernel size as needed
    // blur.apply(image);

    // Roberts Cross operator kernels
    const std::vector<std::vector<int>> robertsKernelX = { {1, 0}, {0, -1} };
    const std::vector<std::vector<int>> robertsKernelY = { {0, 1}, {-1, 0} };

    // Apply Roberts Cross operator 
    applyEdgeDetection(image, robertsKernelX, robertsKernelY);
}

/**
 * Generic function to apply edge detection using specified kernels.
 * This function convolves the image data with both X and Y kernels.
 *
 * @param image The image to apply edge detection on.
 * @param kernelX The kernel to convolve with in the X direction.
 * @param kernelY The kernel to convolve with in the Y direction.
 */
void EdgeDetection::applyEdgeDetection(Image& image, const std::vector<std::vector<int>>& kernelX, const std::vector<std::vector<int>>& kernelY) {
    int width = image.getWidth();
    int height = image.getHeight();
    int channels = image.getChannels();
    unsigned char* data = image.getData();

    int kernelSize = kernelX.size(); // Assuming square kernel

    // Create a vector to store the new pixel data
    std::vector<unsigned char> newData(width * height * channels);

    // Iterate over each pixel
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            for (int c = 0; c < channels; ++c) {
                int gradientX = 0;
                int gradientY = 0;

                // Convolve with kernel
                for (int ky = 0; ky < kernelSize; ++ky) {
                    for (int kx = 0; kx < kernelSize; ++kx) {
                        int nx = x + kx - kernelSize / 2;
                        int ny = y + ky - kernelSize / 2;

                        // Ensure pixel coordinates are within bounds
                        if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                            int index = (ny * width + nx) * channels + c;
                            gradientX += data[index] * kernelX[ky][kx];
                            gradientY += data[index] * kernelY[ky][kx];
                        }
                    }
                }

                // Compute gradient magnitude
                int gradientMagnitude = std::sqrt(gradientX * gradientX + gradientY * gradientY);

                // Store gradient magnitude in newData
                newData[(y * width + x) * channels + c] = std::min(std::max(gradientMagnitude, 0), 255);
            }
        }
    }

    // Copy newData back to image data
    std::copy(newData.begin(), newData.end(), data);
}

/**
 * Interactive method to choose and apply an edge detection method to an image.
 * The method is selected based on the provided integer choice.
 *
 * @param image The image to apply edge detection on.
 * @param choice The integer representing the user's choice of edge detection method.
 */
void EdgeDetection::applyEdge(Image& image, int choice) {
    EdgeOperator operatorType;
    // int choice;
    // std::cout << "Choose edge detection method:\n1. Sobel\n2. Prewitt\n3. Scharr\n4. Roberts Cross\nEnter choice (1-4): ";
    // std::cin >> choice;

    switch (choice) {
    case 1:
        operatorType = Sobel;
        applySobel(image);
        break;
    case 2:
        operatorType = Prewitt;
        applyPrewitt(image);
        break;
    case 3:
        operatorType = Scharr;
        applyScharr(image);
        break;
    case 4:
        operatorType = RobertsCross;
        applyRobertsCross(image);
        break;
    default:
        std::cerr << "Invalid choice." << std::endl;
        return;
    }
}

/**
 * Converts an integer choice to a corresponding EdgeOperator enum value.
 *
 * @param choice The integer choice representing an edge operator.
 * @return EdgeOperator corresponding to the given choice.
 * @throws std::invalid_argument If the choice is invalid.
 */
EdgeOperator EdgeDetection::getEdgeOperatorFromChoice(int choice) {
    switch (choice) {
    case 1:
        return Sobel;
    case 2:
        return Prewitt;
    case 3:
        return Scharr;
    case 4:
        return RobertsCross;
    default:
        throw std::invalid_argument("Invalid choice for edge detection");
    }
}