#include "Image.h"
#include "ImageBlur.h"
#include <iostream>
#include <filesystem>
#include <string>

namespace fs = std::__fs::filesystem; // Using the filesystem namespace for directory operations

int main() {
    Image image;
    // Fixed input path
    std::string inputFileName = "/Users/st2923/Desktop/c++/group_project/advanced-programming-group-selection-sort/Images/vh_anatomy.png";

    if (!image.loadImage(inputFileName)) {
        std::cerr << "Failed to load image: " << inputFileName << std::endl;
        return 1;
    }

    int kernelSize = 5; // Fixed kernel size for simplicity
    int choice;
    std::cout << "Choose blur method:\n1. Box Blur\n2. Median Blur\n3. Gaussian Blur\nEnter choice (1-3): ";
    std::cin >> choice;

    // Determine the project directory by using the input file's directory and navigating up to the project root
    fs::path projectDir = fs::path(inputFileName).parent_path().parent_path(); // Go up twice from Images to project root
    fs::path outputDir = projectDir / "results"; // Construct the path to the results directory
    fs::create_directory(outputDir); // Ensure the "results" directory exists

    ImageBlur *blur;
    std::string blurTypeStr;
    switch(choice) {
        case 1:
            blur = new ImageBlur(Box, kernelSize);
            blurTypeStr = "Box";
            break;
        case 2:
            blur = new ImageBlur(Median, kernelSize);
            blurTypeStr = "Median";
            break;
        case 3:
            blur = new ImageBlur(Gaussian, kernelSize);
            blurTypeStr = "Gaussian";
            break;
        default:
            std::cerr << "Invalid choice." << std::endl;
            return 1;
    }

    blur->apply(image);
    delete blur; // Remember to delete the dynamically allocated blur object

    // Construct the output file name
    std::string baseFileName = fs::path(inputFileName).stem().string(); // Extract base name without extension
    std::string outputFileName = (outputDir / (baseFileName + "_" + blurTypeStr + ".png")).string();
    if (!image.saveImage(outputFileName, "png")) {
        std::cerr << "Failed to save image: " << outputFileName << std::endl;
        return 1;
    }

    std::cout << "Processed image saved to " << outputFileName << std::endl;
    return 0;
}
