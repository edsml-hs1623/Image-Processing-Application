#include "User_unitTests.h"
#include "ColourCorrectionTest.h"
#include "EdgeDetectionTest.h"
#include "ImageBlurTest.h"
#include "ThreeDFilterTest.h"
#include "ProjectionTest.h"

#include <iostream>
#include <vector>
#include <string>

void runColourCorrectionTests() {
    std::vector<std::string> colourTests = {
            "Grayscale Conversion",
            "Brightness Adjustment",
            "Histogram Equalization",
            "Thresholding",
            "Salt and Pepper Noise",
            "Back to Main Menu"
    };

    int choice = 0;
    while (choice != colourTests.size()) {
        std::cout << "\nSelect a Colour Correction Test to run:\n";
        for (size_t i = 0; i < colourTests.size(); ++i) {
            std::cout << (i + 1) << ". " << colourTests[i] << '\n';
        }

        std::cout << "Enter a number (1-" << colourTests.size() << "): ";
        std::cin >> choice;

        if (choice < 1 || choice > colourTests.size()) {
            std::cout << "Invalid choice, please try again.\n";
            continue;
        }

        if (choice == colourTests.size()) break; // Back to Main Menu option

        ColourCorrectionTest test;
        test.run(choice - 1);
    }
}

void runImageBlurTests() {
    std::vector<std::string> blurTests = {
            "Median Blur",
            "Box Blur",
            "Gaussian Blur",
            "Back to Main Menu"
    };

    int choice = 0;
    while (choice != blurTests.size()) {
        std::cout << "\nSelect an Image Blur Test to run:\n";
        for (size_t i = 0; i < blurTests.size(); ++i) {
            std::cout << (i + 1) << ". " << blurTests[i] << '\n';
        }

        std::cout << "Enter a number (1-" << blurTests.size() << "): ";
        std::cin >> choice;

        if (choice < 1 || choice > blurTests.size()) {
            std::cout << "Invalid choice, please try again.\n";
            continue;
        }

        if (choice == blurTests.size()) break; // Back to Main Menu option

        ImageBlurTest test;
        test.run(choice - 1);
    }
}

void runEdgeDetectionTests() {
    std::vector<std::string> edgeTests = {
            "Sobel Operator",
            "Prewitt Operator",
            "Scharr Operator",
            "Roberts Cross Operator",
            "Back to Main Menu"
    };

    int choice = 0;
    while (choice != edgeTests.size()) {
        std::cout << "\nSelect an Edge Detection Test to run:\n";
        for (size_t i = 0; i < edgeTests.size(); ++i) {
            std::cout << (i + 1) << ". " << edgeTests[i] << '\n';
        }

        std::cout << "Enter a number (1-" << edgeTests.size() << "): ";
        std::cin >> choice;

        if (choice < 1 || choice > edgeTests.size()) {
            std::cout << "Invalid choice, please try again.\n";
            continue;
        }

        if (choice == edgeTests.size()) break; // Back to Main Menu option

        EdgeDetectionTest test;
        test.run(choice - 1);
    }
}

void runThreeDFilterTests() {
    std::vector<std::string> filterTests = {
            "Gaussian Blur",
            "Median Blur",
            "Back to Main Menu"
    };

    int choice = 0;
    while (choice != filterTests.size()) {
        std::cout << "\nSelect a 3D Filter Test to run:\n";
        for (size_t i = 0; i < filterTests.size(); ++i) {
            std::cout << (i + 1) << ". " << filterTests[i] << '\n';
        }

        std::cout << "Enter a number (1-" << filterTests.size() << "): ";
        std::cin >> choice;

        if (choice < 1 || choice > filterTests.size()) {
            std::cout << "Invalid choice, please try again.\n";
            continue;
        }

        if (choice == filterTests.size()) break; // Back to Main Menu option

        ThreeDFilterTest threeDFilterTest;
        threeDFilterTest.run(choice - 1);
    }
}

void runProjectionTests() {
    std::vector<std::string> projectionTests = {
            "MIP (Maximum Intensity Projection)",
            "MINIP (Minimum Intensity Projection)",
            "AIP (Average Intensity Projection)",
            "Back to Main Menu"
    };

    int choice = 0;
    while (choice != projectionTests.size()) {
        std::cout << "\nSelect a Projection Test to run:\n";
        for (size_t i = 0; i < projectionTests.size(); ++i) {
            std::cout << (i + 1) << ". " << projectionTests[i] << '\n';
        }

        std::cout << "Enter a number (1-" << projectionTests.size() << "): ";
        std::cin >> choice;

        if (choice < 1 || choice > projectionTests.size()) {
            std::cout << "Invalid choice, please try again.\n";
            continue;
        }

        if (choice == projectionTests.size()) break; // Back to Main Menu option

        ProjectionTest projectionTest;
        projectionTest.run(choice - 1);
    }
}

void User_unitTests::run() {
    std::vector<std::string> tests = {
            "Colour Correction Tests",
            "Image Blur Tests",
            "Edge Detection Tests",
            "3D Filter Tests",
            "Projection Tests",
            "Exit"
    };

    int choice = 0;

    while (choice != tests.size()) {
        std::cout << "\nSelect a test category to run:\n";
        for (size_t i = 0; i < tests.size(); ++i) {
            std::cout << (i + 1) << ". " << tests[i] << '\n';
        }

        std::cout << "Enter a number (1-" << tests.size() << "): ";
        std::cin >> choice;

        if (choice < 1 || choice > tests.size()) {
            std::cout << "Invalid choice, please try again.\n";
            continue;
        }

        if (choice == tests.size()) break; // Exit option

        switch (choice) {
            case 1:
                runColourCorrectionTests();
                break;
            case 2:
                runImageBlurTests();
                break;
            case 3:
                runEdgeDetectionTests();
                break;
            case 4:
                runThreeDFilterTests();
                break;
            case 5:
                runProjectionTests();
                break;
        }
    }

    std::cout << "Exiting unit tests.\n";
}