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
#include "User_3D.h"
#include "User_unitTests.h"

#include <iostream>
#include <filesystem>
#include <string>


int main() {
    int choice;
    std::cout << "Please select the mode you would like to use:" << std::endl;
    std::cout << "1. 2D Processing" << std::endl;
    std::cout << "2. 3D Processing" << std::endl;
    std::cout << "3. Unit Tests" << std::endl;
    std::cout << "Enter choice (1, 2 or 3): ";
    std::cin >> choice;

    // Based on the user's choice, create the appropriate user object
    // and call the run method to start the interaction.
    if (choice == 1) {
        User_2D();
    } else if (choice == 2) {
        User_3D user3D;
        user3D.run();
    } else if (choice == 3) {
        User_unitTests unitTests;
        unitTests.run();
    } else {
        std::cout << "Invalid choice. Exiting program." << std::endl;
        return 1;
    }

    return 0;
}