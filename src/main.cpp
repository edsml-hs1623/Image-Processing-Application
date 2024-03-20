/**
 * @file main.cpp
 *
 * @brief Main entry point for the image processing application.
 *
 * This file serves as the entry point for the image processing application. It initializes
 * the user interface defined in User_2D.h, facilitating user interaction and execution of
 * various image processing techniques such as edge detection, blurring, and color correction.
 * The main function calls User_2D(), which in turn invokes the appropriate functions based on user input.
 *
 * Key Features:
 *   - Initiates the user interface for image processing.
 *   - Allows users to select and apply different processing techniques to images.
 *
 * Usage:
 *   Simply execute the compiled program to start the image processing application.
 *   Follow the prompts in the command line interface to select images and apply desired filters.
 *
 * Dependencies:
 *   - User_2D.h
 *   - Associated image processing classes (Image, EdgeDetection, ImageBlur, ColourCorrection)
 *   - Standard libraries: <iostream>, <filesystem>, <string>
 */

#include "Image.h"
#include "EdgeDetection.h"
#include "ImageBlur.h"
#include "ColourCorrection.h"
#include "User_2D.h"

#include <iostream>
#include <filesystem>
#include <string>


int main() {
    User_2D();
    return 0;
}