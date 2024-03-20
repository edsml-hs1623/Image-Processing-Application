/**
 * @file User_2D.h
 *
 * @brief Header file for user interactions and image processing in the 2D user interface.
 *
 * This header file declares functions for user interaction and applying various image processing
 * techniques like edge detection, blurring, and color correction in a 2D user interface context.
 * It facilitates the selection of images and the application of different processing techniques
 * through user input. The file references multiple image processing classes, including
 * EdgeDetection, ImageBlur, and ColourCorrection.
 *
 * Key Functionalities:
 *   - Interface for selecting images to process.
 *   - Functions to apply various image processing techniques.
 *   - User interaction to guide through the process and save results.
 *
 * @note The file relies on proper setup and inclusion of the associated image processing classes,
 *       and it uses the standard filesystem library for directory and file handling.
 *
 * Dependencies:
 *   - Image.h
 *   - EdgeDetection.h
 *   - ImageBlur.h
 *   - ColourCorrection.h
 *   - <filesystem>, <iostream>, <vector>, and <string>
 */

#ifndef USER_2D_H
#define USER_2D_H

#include <vector>
#include <filesystem>
#include <iostream>
#include <string>

#include "Image.h"
#include "EdgeDetection.h"
#include "ImageBlur.h"
#include "ColourCorrection.h"

namespace fs = std::filesystem;

int selectImage(const std::vector<std::string>& images);
void applyEdgeDetection(const fs::path& projectDir, const fs::path& imagePath, Image& image);
void applyBlur(const fs::path& projectDir, const fs::path& imagePath, Image& image);
void applyColourCorrection(const fs::path& projectDir, const fs::path& imagePath, Image& image, int filter);
void User_2D();

#endif // USER_2D_H