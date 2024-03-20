/**
 * @file User_2D.cpp
 *
 * @brief Implementation of user interactions for applying image processing techniques.
 *
 * This file provides the implementation for a user interface to apply various image processing
 * techniques, including edge detection, blurring, and color correction. The file defines
 * functions to select images from a list, apply specified filters or processing techniques, and
 * save the processed images to a specific directory. It relies on multiple image processing classes
 * such as EdgeDetection, ImageBlur, and ColourCorrection to perform these operations.
 *
 * Key Functionalities:
 *   - User interaction to select images and processing techniques.
 *   - Integration of different image processing filters.
 *   - Saving processed images to designated directories.
 *
 * Usage:
 *   User_2D(); // Invokes the main user interaction function to apply image processing
 *
 * @note This file requires the presence of associated image processing classes and image files
 *       within the specified directories. Ensure that all dependencies are correctly set up in
 *       your project structure.
 *
 * Dependencies:
 *   - Image.h
 *   - EdgeDetection.h
 *   - ImageBlur.h
 *   - ColourCorrection.h
 *   - <filesystem>, <iostream>, <string>, and other standard libraries
 */

#include "User_2D.h"
#include "Image.h"
#include "EdgeDetection.h"
#include "ImageBlur.h"
#include "ColourCorrection.h"

#include <filesystem>
#include <iostream>
#include <string>

/**
 * Presents a list of images to the user and gets their choice.
 *
 * @param images A vector of strings representing the names of the images.
 * @return The index of the chosen image as an integer.
 */
int selectImage(const std::vector<std::string>& images) {
    std::cout << "Select an image to process:\n";
    for (size_t i = 0; i < images.size(); ++i) {
        std::cout << (i + 1) << ". " << images[i] << '\n';
    }

    int imageChoice;
    std::string userInput;
    do {
        std::cout << "Enter a number (1-" << images.size() << "): ";
        std::cin >> userInput;

        try {
            imageChoice = std::stoi(userInput);
            if (imageChoice < 1 || imageChoice > images.size()) {
                std::cerr << "Invalid input, please try again.\n";
            }
        }
        catch (std::invalid_argument const& e) {
            std::cerr << "Invalid input, please enter a number.\n";
            imageChoice = 0; // Resetting imageChoice to trigger the loop again
        }
        catch (std::out_of_range const& e) {
            std::cerr << "Invalid input, number out of range.\n";
            imageChoice = 0; // Resetting imageChoice to trigger the loop again
        }
    } while (imageChoice < 1 || imageChoice > images.size());

    return imageChoice;
}

/**
 * Applies edge detection to a given image and saves the result.
 *
 * @param projectDir The filesystem path to the project directory.
 * @param imagePath The path to the input image.
 * @param image The Image object to which edge detection will be applied.
 */
void applyEdgeDetection(const fs::path& projectDir, const fs::path& imagePath, Image& image) {
    // Construct the path to the results directory and ensure it exists
    fs::path edgeDetectionDir = projectDir / "edgeDetection";
    if (!fs::exists(edgeDetectionDir)) {
        fs::create_directories(edgeDetectionDir);
    }

    // Apply edge detection
    fs::path edgeDetectionTypeDir;
    std::string edgeDetectionTypeStr;

    // Get user's choice and detect invalid input
    int choice;
    bool validChoice = false;
    while (!validChoice) {
        std::cout << "Choose edge detection method:\n1. Sobel\n2. Prewitt\n3. Scharr\n4. Roberts Cross\nEnter choice (1-4): ";
        std::string userInput;
        std::cin >> userInput;

        try {
            choice = std::stoi(userInput);
            if (choice >= 1 && choice <= 4) {
                validChoice = true;
            }
            else {
                std::cerr << "Invalid choice, please try again." << std::endl;
            }
        }
        catch (std::invalid_argument const& e) {
            std::cerr << "Invalid input, please enter a number." << std::endl;
        }
        catch (std::out_of_range const& e) {
            std::cerr << "Invalid input, number out of range." << std::endl;
        }
    }

    // Switch different types of edge detection 
    switch (choice) {
    case 1:
        edgeDetectionTypeDir = edgeDetectionDir / "Sobel";
        edgeDetectionTypeStr = "Sobel";
        break;
    case 2:
        edgeDetectionTypeDir = edgeDetectionDir / "Prewitt";
        edgeDetectionTypeStr = "Prewitt";
        break;
    case 3:
        edgeDetectionTypeDir = edgeDetectionDir / "Scharr";
        edgeDetectionTypeStr = "Scharr";
        break;
    case 4:
        edgeDetectionTypeDir = edgeDetectionDir / "Roberts Cross";
        edgeDetectionTypeStr = "Roberts Cross";
        break;
    }

    EdgeOperator edgeOperator = EdgeDetection::getEdgeOperatorFromChoice(choice);
    EdgeDetection edgeDetector(edgeOperator);
    edgeDetector.applyEdge(image, choice);

    // Ensure the directory exists
    if (!fs::exists(edgeDetectionTypeDir)) {
        fs::create_directories(edgeDetectionTypeDir);
    }

    // Save the processed image to corresponding folder 
    std::string baseFileName = fs::path(imagePath).stem().string(); // Extract base name without extension
    std::string outputFileName = (edgeDetectionTypeDir / (baseFileName + "_" + edgeDetectionTypeStr + ".png")).string();

    if (!image.saveImage(outputFileName, "png")) {
        std::cerr << "Failed to save image: " << outputFileName << std::endl;
        return;
    }
    std::cout << "Processed image saved to " << outputFileName << std::endl;
}

/**
 * Applies a blur effect to a given image and saves the result.
 *
 * @param projectDir The filesystem path to the project directory.
 * @param imagePath The path to the input image.
 * @param image The Image object to which the blur effect will be applied.
 */
void applyBlur(const fs::path& projectDir, const fs::path& imagePath, Image& image) {
    // Get user's choice of blur methods and detect invalid input
    int choice;
    bool validChoice = false;
    while (!validChoice) {
        std::cout << "Choose blur method:\n1. Box Blur\n2. Median Blur\n3. Gaussian Blur\nEnter choice (1-3): ";
        std::string userInput;
        std::cin >> userInput;

        try {
            choice = std::stoi(userInput);
            if (choice >= 1 && choice <= 3) {
                validChoice = true;
            }
            else {
                std::cerr << "Invalid choice, please try again." << std::endl;
            }
        }
        catch (std::invalid_argument const& e) {
            std::cerr << "Invalid input, please enter a number." << std::endl;
        }
        catch (std::out_of_range const& e) {
            std::cerr << "Invalid input, number out of range." << std::endl;
        }
    }

    int kernelSize;
    std::cout << "Enter kernel size (e.g., 3 for 3x3): ";
    std::cin >> kernelSize;

    // Construct the path to the results directory and ensure it exists
    fs::path blurDir = projectDir / "blur";
    if (!fs::exists(blurDir)) {
        fs::create_directories(blurDir);
    }

    fs::path blurTypeDir; // Directory path for the specific blur type
    std::string blurTypeStr;
    switch (choice) {
    case 1:
        blurTypeDir = blurDir / "box";
        blurTypeStr = "Box";
        break;
    case 2:
        blurTypeDir = blurDir / "median";
        blurTypeStr = "Median";
        break;
    case 3:
        blurTypeDir = blurDir / "gaussian";
        blurTypeStr = "Gaussian";
        break;
    }

    // Ensure the directory exists
    if (!fs::exists(blurTypeDir)) {
        fs::create_directories(blurTypeDir);
    }

    // validate user's choice and create the blur object
    ImageBlur* blur = nullptr;
    switch (choice) {
    case 1:
        blur = new ImageBlur(Box, kernelSize);
        break;
    case 2:
        blur = new ImageBlur(Median, kernelSize);
        break;
    case 3:
        blur = new ImageBlur(Gaussian, kernelSize);
        break;
    }

    // Check if blur was successfully created before proceeding
    if (!blur) {
        std::cerr << "Blur object creation failed." << std::endl;
        return;
    }

    blur->apply(image);
    delete blur;

    // Save the processed image to corresponding folder 
    std::string baseFileName = fs::path(imagePath).stem().string();
    std::string kernelSizeStr = std::to_string(kernelSize) + "x" + std::to_string(kernelSize);
    std::string outputFileName = (blurTypeDir / (baseFileName + "_" + kernelSizeStr + ".png")).string();

    if (!image.saveImage(outputFileName, "png")) {
        std::cerr << "Failed to save image: " << outputFileName << std::endl;
        return;
    }
    std::cout << "Processed image saved to " << outputFileName << std::endl;
}

/**
 * Applies color correction to a given image and saves the result.
 *
 * @param projectDir The filesystem path to the project directory.
 * @param imagePath The path to the input image.
 * @param image The Image object to which color correction will be applied.
 * @param filter The type of color correction filter to be applied.
 */
void applyColourCorrection(const fs::path& projectDir, const fs::path& imagePath, Image& image, int filter) {
    ColorSpace colorSpace = ColorSpace::HSV; // Default to HSV
    std::string histogramType;
    if ((filter == 3 || filter == 4) && image.getChannels() != 1) {
        std::cout << "Choose color space for processing:\n"
            << "1. HSL\n"
            << "2. HSV\n"
            << "Enter choice (1-2): ";
        int colorSpaceChoice;
        std::cin >> colorSpaceChoice;
        if (colorSpaceChoice == 1) {
            histogramType = "HSL";
        }
        else {
            histogramType = "HSV";
        }
        colorSpace = (colorSpaceChoice == 1) ? ColorSpace::HSL : ColorSpace::HSV;
    }

    int parameter = 0; // For methods that require an additional parameter
    if (filter == 2 || filter == 4 || filter == 5) {
        std::cout << "Enter parameter value (brightness level, threshold value, or noise percentage): ";
        std::cin >> parameter;
    }

    ColourCorrectionType correctionType = static_cast<ColourCorrectionType>(filter - 1);
    ColourCorrection correction(correctionType, parameter, colorSpace);

    // Apply the selected colour correction
    correction.apply(image);

    fs::path ColourCorrectionTypeDir;
    std::string ColourCorrectionTypeStr;
    switch (filter) {
    case 1:
        ColourCorrectionTypeDir = projectDir / "grayscale";
        ColourCorrectionTypeStr = "";

        break;
    case 2:
        ColourCorrectionTypeDir = projectDir / "brightness";
        if (parameter > 0) {
            ColourCorrectionTypeStr = "_plus" + std::to_string(parameter);
        }
        else {
            ColourCorrectionTypeStr = "_minus" + std::to_string(std::abs(parameter));
        }
        break;
    case 3:
        ColourCorrectionTypeDir = projectDir / "histogram";
        ColourCorrectionTypeStr = "_" + histogramType;
        break;
    case 4:
        ColourCorrectionTypeDir = projectDir / "threshold";
        if (histogramType.empty()) {
            ColourCorrectionTypeStr = "_" + std::to_string(parameter);
        }
        else {
            ColourCorrectionTypeStr = "_" + histogramType + "_" + std::to_string(parameter);
        }
        break;
    case 5:
        ColourCorrectionTypeDir = projectDir / "saltandpepper";
        ColourCorrectionTypeStr = "_" + std::to_string(parameter);
        break;
    }
    // Ensure the directory exists
    if (!fs::exists(ColourCorrectionTypeDir)) {
        fs::create_directories(ColourCorrectionTypeDir);
    }

    std::string baseFileName = fs::path(imagePath).stem().string(); // Extract base name without extension
    std::string outputFileName = (ColourCorrectionTypeDir / (baseFileName + ColourCorrectionTypeStr + ".png")).string();

    if (!image.saveImage(outputFileName, "png")) {
        std::cerr << "Failed to save corrected image: " << outputFileName << std::endl;
        return;
    }

    std::cout << "Corrected image saved to " << outputFileName << std::endl;
}

/**
 * Main function to interact with the user for applying various image processing techniques.
 * Allows the user to choose an image and apply different processing filters such as blur,
 * edge detection, and color correction.
 */
void User_2D() {
    // Present the list of images here
    std::vector<std::string> images = {
            "dimorphos.png",
            "gracehopper.png",
            "stinkbug.png",
            "tienshan.png",
            "vh_anatomy.png",
            "vh_anatomy_sp15.png",
            "vh_ct.png"
    };

    // Ask user which image do they want to process
    int imageChoice = selectImage(images);

    // Adjust the image file name based on user choice
    std::string chosenImageFileName = images[imageChoice - 1]; // Subtract 1 for correct index

    // Construct the full path to the image
    fs::path imagePath = fs::path("../Images") / chosenImageFileName;
    Image image;
    if (!image.loadImage(imagePath.string())) {
        std::cerr << "Failed to load image: " << imagePath.string() << std::endl;
        return;
    }

    // Determine the project directory
    fs::path projectDir = fs::path(imagePath).parent_path().parent_path() / "Output";  // Go up twice from Images to project root



    // Choose 7 different filters based on user's choice
    int filter;
    std::cout << "Choose filter:\n1. grayscale\n2. brightness\n3. histogram\n4. threshold\n5. salt and pepper\n6. blur\n7. edge detection\nEnter choice (1-7): ";
    std::cin >> filter;

    switch (filter) {
    // Blur filter
    case 6: {
        applyBlur(projectDir, imagePath, image);
        break;
    }

    // Edge detection
    case 7: {
        applyEdgeDetection(projectDir, imagePath, image);
        break;
    }

    // Deal with other choices (1-5)
    default: {
        applyColourCorrection(projectDir, imagePath, image, filter);
        break;
    }
    }
}