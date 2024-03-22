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
    std::cout << "\nSelect an image to process:\n";
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
            // Check for 'Save and Exit' option
            if (imageChoice == images.size()) {
                return imageChoice; // Return the choice to exit the loop
            }
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
 * @param multiple Flag indicating whether to apply multiple filters.
 */
std::string applyEdgeDetection(const fs::path& projectDir, const fs::path& imagePath, Image& image, int multiple) {
    // Construct the path to the results directory and ensure it exists
    fs::path edgeDetectionDir = projectDir / "7-edgedetection";
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
        std::cout << "\nChoose edge detection method:\n1. Sobel\n2. Prewitt\n3. Scharr\n4. Roberts Cross\nEnter choice (1-4): ";
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
            edgeDetectionTypeDir = edgeDetectionDir / "sobel";
            edgeDetectionTypeStr = "sobel";
            break;
        case 2:
            edgeDetectionTypeDir = edgeDetectionDir / "prewitt";
            edgeDetectionTypeStr = "prewitt";
            break;
        case 3:
            edgeDetectionTypeDir = edgeDetectionDir / "scharr";
            edgeDetectionTypeStr = "scharr";
            break;
        case 4:
            edgeDetectionTypeDir = edgeDetectionDir / "robertscross";
            edgeDetectionTypeStr = "robertscross";
            break;
    }

    EdgeOperator edgeOperator = EdgeDetection::getEdgeOperatorFromChoice(choice);
    EdgeDetection edgeDetector(edgeOperator);
    edgeDetector.applyEdge(image, choice);

    // Ensure the directory exists
    if (!fs::exists(edgeDetectionTypeDir)) {
        fs::create_directories(edgeDetectionTypeDir);
    }

    std::string baseFileName;
    std::string outputFileName;
    std::string suffix;

    if (multiple == 1){
        // Save the processed image to corresponding folder
        std::string baseFileName = fs::path(imagePath).stem().string(); // Extract base name without extension
        std::string outputFileName = (edgeDetectionTypeDir / (baseFileName + "_" + edgeDetectionTypeStr + ".png")).string();

        if (!image.saveImage(outputFileName, "png")) {
            std::cerr << "Failed to save image: " << outputFileName << std::endl;
        }
        std::cout << "\nProcessed image saved to " << outputFileName << std::endl;
        return "";
    }
    else{
        return edgeDetectionTypeStr;
    }
}


/**
 * Applies a blur effect to a given image and saves the result.
 *
 * @param projectDir The filesystem path to the project directory.
 * @param imagePath The path to the input image.
 * @param image The Image object to which the blur effect will be applied.
 * @param multiple Flag indicating whether to apply multiple filters.
 */
std::string applyBlur(const fs::path& projectDir, const fs::path& imagePath, Image& image, int multiple) {
    // Get user's choice of blur methods and detect invalid input
    int choice;
    bool validChoice = false;
    while (!validChoice) {
        std::cout << "\nChoose blur method:\n1. Box Blur\n2. Median Blur\n3. Gaussian Blur\nEnter choice (1-3): ";
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
    fs::path blurDir = projectDir / "6-blur";
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
        return "";
    }

    blur->apply(image);
    delete blur;

    std::string kernelSizeStr = std::to_string(kernelSize) + "x" + std::to_string(kernelSize);

    if (multiple == 1){
        // Save the processed image to corresponding folder
        std::string baseFileName = fs::path(imagePath).stem().string();
        std::string outputFileName = (blurTypeDir / (baseFileName + "_" + kernelSizeStr + ".png")).string();

        if (!image.saveImage(outputFileName, "png")) {
            std::cerr << "Failed to save image: " << outputFileName << std::endl;
        }
        std::cout << "\nProcessed image saved to " << outputFileName << std::endl;
        return "";
    }
    else{
        return kernelSizeStr;
    }
}


/**
 * Applies color correction to a given image and saves the result.
 *
 * @param projectDir The filesystem path to the project directory.
 * @param imagePath The path to the input image.
 * @param image The Image object to which color correction will be applied.
 * @param filter The type of color correction filter to be applied.
 * @param multiple Flag indicating whether to apply multiple filters.
 */
std::string applyColourCorrection(const fs::path& projectDir, const fs::path& imagePath, Image& image, int filter, int multiple) {
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
            ColourCorrectionTypeDir = projectDir / "1-grayscale";
            ColourCorrectionTypeStr = "";

            break;
        case 2:
            ColourCorrectionTypeDir = projectDir / "2-brightness";
            if (parameter > 0) {
                ColourCorrectionTypeStr = "_plus" + std::to_string(parameter);
            }
            else {
                ColourCorrectionTypeStr = "_minus" + std::to_string(std::abs(parameter));
            }
            break;
        case 3:
            ColourCorrectionTypeDir = projectDir / "3-histogram";
            ColourCorrectionTypeStr = "_" + histogramType;
            break;
        case 4:
            ColourCorrectionTypeDir = projectDir / "4-threshold";
            if (histogramType.empty()) {
                ColourCorrectionTypeStr = "_" + std::to_string(parameter);
            }
            else {
                ColourCorrectionTypeStr = "_" + histogramType + "_" + std::to_string(parameter);
            }
            break;
        case 5:
            ColourCorrectionTypeDir = projectDir / "5-saltandpepper";
            ColourCorrectionTypeStr = "_" + std::to_string(parameter);
            break;
    }
    // Ensure the directory exists
    if (!fs::exists(ColourCorrectionTypeDir)) {
        fs::create_directories(ColourCorrectionTypeDir);
    }

    if (multiple == 1){
        std::string baseFileName = fs::path(imagePath).stem().string(); // Extract base name without extension
        std::string outputFileName = (ColourCorrectionTypeDir / (baseFileName + ColourCorrectionTypeStr + ".png")).string();

        if (!image.saveImage(outputFileName, "png")) {
            std::cerr << "Failed to save corrected image: " << outputFileName << std::endl;
        }
        std::cout << "\nProcessed image saved to " << outputFileName << std::endl;
        return "";
    }
    else{
        return ColourCorrectionTypeStr;
    }
}


/**
 * Retrieves the suffix for a color correction filter.
 *
 * @param filter The filter index.
 * @return The suffix corresponding to the color correction filter.
 */
std::string getColourCorrectionSuffix (int filter){
    switch(filter){
        case 1: return "grayscale";
        case 2: return "brightness";
        case 3: return "histogram";
        case 4: return "threshold";
        case 5: return "saltandpepper";
        default: return "";
    }
}


/**
 * Main function to interact with the user for applying various image processing techniques.
 * Allows the user to choose an image and apply different processing filters such as blur,
 * edge detection, and color correction.
 */
void User_2D() {

    // Define the path to the directory containing images
    fs::path imagesDir = fs::path("../Images");

    // Check if the directory exists
    if (!fs::exists(imagesDir) || !fs::is_directory(imagesDir)) {
        std::cerr << "Images directory not found or is not a valid directory." << std::endl;
        return;
    }

    // Collect the names of image files in the directory
    std::vector<std::string> images;
    for (const auto& entry : fs::directory_iterator(imagesDir)) {
        if (entry.is_regular_file()) {
            images.push_back(entry.path().filename().string());
        }
    }

    // Add the "Save and Exit" option
    images.push_back("Save and Exit");



    // Using single filter or multiple filters
    int multiple;
    std::cout << "\nChoose single filter or multiple filters:\n"
              << "1. single\n"
              << "2. multiple\n"
              << "Enter 1 or 2: ";
    std::cin >> multiple;
    switch (multiple){

        // Single filter
        case 1: {

            // Loop until the user chooses the "Save and Exit" option
            while (true) {
                // Ask user which image do they want to process
                int imageChoice = selectImage(images);

                // Check if the user chose to exit
                if (imageChoice == images.size()) {
                    std::cout << "Exiting the program." << std::endl;
                    break; // Exit the loop and the function
                }

                // Adjust the image file name based on user choice
                std::string chosenImageFileName = images[imageChoice - 1]; // Subtract 1 for correct index

                // Construct the full path to the image
                fs::path imagePath = fs::path("../Images") / chosenImageFileName;
                Image image;
                if (!image.loadImage(imagePath.string())) {
                    std::cerr << "Failed to load image: " << imagePath.string() << std::endl;
                    continue; // If image cannot be loaded, continue with the next iteration
                }

                // Determine the project directory
                fs::path projectDir =
                        fs::path(imagePath).parent_path().parent_path() / "Output";  // Go up twice from Images to project root

                // Choose 7 different filters based on user's choice
                int filter;
                std::cout << "\nChoose filter:\n1. grayscale\n2. brightness\n3. histogram\n4. threshold\n5. salt and pepper\n6. blur\n7. edge detection\nEnter choice (1-7): ";
                std::cin >> filter;

                switch (filter) {
                    // Blur filter
                    case 6: {
                        applyBlur(projectDir, imagePath, image, multiple);
                        break;
                    }

                        // Edge detection
                    case 7: {
                        applyEdgeDetection(projectDir, imagePath, image, multiple);
                        break;
                    }

                        // Deal with other choices (1-5)
                    default: {
                        applyColourCorrection(projectDir, imagePath, image, filter, multiple);
                        break;
                    }
                }
            }
            break;
        }


            // Multiple filters
        case 2: {
            // Ask user which image do they want to process
            int imageChoice = selectImage(images);

            // Adjust the image file name based on user choice
            std::string chosenImageFileName = images[imageChoice - 1]; // Subtract 1 for correct index

            // Construct the full path to the image
            fs::path imagePath = fs::path("../Images") / chosenImageFileName;
            Image image;
            if (!image.loadImage(imagePath.string())) {
                std::cerr << "Failed to load image: " << imagePath.string() << std::endl;
            }

            // Determine the project directory
            fs::path projectDir =
                    fs::path(imagePath).parent_path().parent_path() / "Output";  // Go up twice from Images to project root

            // the loop for multiple filter
            // create a folder for these images
            fs::path othersDir = projectDir / "others";
            if (!fs::exists(othersDir)) {
                fs::create_directories(othersDir);
            }

            // to keep all the names
            std::string suffixStr;
            std::string suffix;


            // Loop until the user chooses the "Save and Exit" option
            while (true){
                int filter;
                std::cout << "\nChoose filter:\n1. grayscale\n2. brightness\n3. histogram\n4. threshold\n5. salt and pepper\n6. blur\n7. edge detection\n8. Save and Exit\nEnter choice (1-8): ";
                std::cin >> filter;

                if (filter == 8){
                    break;
                }

                // Applied the filter and save the suffix
                switch (filter) {
                    // Blur filter
                    case 6: {
                        suffix = "blur_";
                        suffix += applyBlur(projectDir, imagePath, image, multiple);
                        break;
                    }

                        // Edge detection
                    case 7: {
                        suffix = "edgeDetection_";
                        suffix += applyEdgeDetection(projectDir, imagePath, image, multiple);
                        break;
                    }

                        // Deal with other choices (1-5)
                    default: {
                        suffix = getColourCorrectionSuffix(filter);
                        suffix += applyColourCorrection(projectDir, imagePath, image, filter, multiple);
                        break;
                    }
                }
                suffixStr += "_";
                suffixStr += suffix;
            }

            // Put every suffix together and save it
            std::string baseFileName = fs::path(imagePath).stem().string(); // Extract base name without extension
            std::string outputFileName = (othersDir / (baseFileName + suffixStr + ".png")).string();

            if (!image.saveImage(outputFileName, "png")) {
                std::cerr << "Failed to save corrected image: " << outputFileName << std::endl;
                return;
            }

            std::cout << "\nProcessed image saved to " << outputFileName << std::endl;

            break;
        }
    }
}