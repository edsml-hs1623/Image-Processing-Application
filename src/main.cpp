#include "Image.h"
#include "ImageBlur.h"
#include <iostream>
#include <filesystem>
#include <string>

namespace fs = std::__fs::filesystem; // Using the filesystem namespace for directory operations

int main() {

    std::vector<std::string> images = {
            "dimorphos.png",
            "gracehopper.png",
            "stinkbug.png",
            "tienshan.png",
            "vh_anatomy.png",
            "vh_anatomy_sp15.png",
            "vh_ct.png" // Assuming this is an additional image
    };

    // Present the list of images
    std::cout << "Select an image to process:\n";
    for (size_t i = 0; i < images.size(); ++i) {
        std::cout << (i + 1) << ". " << images[i] << '\n';
    }

    // Get the user's choice
    int imageChoice;
    do {
        std::cout << "Enter a number (1-" << images.size() << "): ";
        std::cin >> imageChoice;
    } while(imageChoice < 1 || imageChoice > images.size());

    Image image;
    std::string inputFileName = "/Users/st2923/Desktop/c++/group_project/advanced-programming-group-selection-sort/Images/vh_anatomy.png";

    if (!image.loadImage(inputFileName)) {
        std::cerr << "Failed to load image: " << inputFileName << std::endl;
        return 1;
    }

    int choice;
    std::cout << "Choose blur method:\n1. Box Blur\n2. Median Blur\n3. Gaussian Blur\nEnter choice (1-3): ";
    std::cin >> choice;

    int kernelSize;
    std::cout << "Enter kernel size (e.g., 3 for 3x3): ";
    std::cin >> kernelSize;

    fs::path projectDir = fs::path(inputFileName).parent_path().parent_path();
    fs::path blurTypeDir; // Directory path for the specific blur type
    std::string blurTypeStr;
    switch(choice) {
        case 1:
            blurTypeDir = projectDir / "6-blur" / "box";
            blurTypeStr = "Box";
            break;
        case 2:
            blurTypeDir = projectDir / "6-blur" / "median";
            blurTypeStr = "Median";
            break;
        case 3:
            blurTypeDir = projectDir / "6-blur" / "gaussian";
            blurTypeStr = "Gaussian";
            break;
        default:
            std::cerr << "Invalid choice." << std::endl;
            return 1;
    }

    // Ensure the directory exists
    if (!fs::exists(blurTypeDir)) {
        fs::create_directories(blurTypeDir);
    }

    ImageBlur *blur = new ImageBlur(static_cast<BlurType>(choice), kernelSize);
    blur->apply(image);
    delete blur;

    std::string baseFileName = fs::path(inputFileName).stem().string();
    std::string kernelSizeStr = std::to_string(kernelSize) + "x" + std::to_string(kernelSize);
    std::string outputFileName = (blurTypeDir / (baseFileName + "_" + kernelSizeStr + ".png")).string();
    if (!image.saveImage(outputFileName, "png")) {
        std::cerr << "Failed to save image: " << outputFileName << std::endl;
        return 1;
    }

    std::cout << "Processed image saved to " << outputFileName << std::endl;
    return 0;
}
