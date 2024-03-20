#include "Volume.h"
#include "Projection.h"
#include "ThreeDFilter.h"
#include <filesystem>
#include <iostream>
#include "Slice.h"
#include "User_3d.h"

namespace fs = std::filesystem;

int main() {
    std::string baseDir = "../Scans"; // Adjust this path as necessary

    // User prompt for dataset selection
    std::cout << "Select the dataset you want to process:\n";
    std::cout << "1. Fracture\n";
    std::cout << "2. Confuciusornis\n";
    std::cout << "Enter your choice (1 or 2): ";

    int choice;
    std::cin >> choice;

    std::string datasetName;
    switch (choice) {
        case 1:
            datasetName = "fracture";
            break;
        case 2:
            datasetName = "confuciusornis";
            break;
        default:
            std::cerr << "Invalid choice. Exiting program.\n";
            return -1;
    }

    // Create an instance of User_3D with the selected dataset
    User_3D user3D(baseDir, datasetName);

    // Run the process
    user3D.run();

    return 0;
}

