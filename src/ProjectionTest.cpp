#include "ProjectionTest.h"
#include "Volume.h"
#include "Projection.h"
#include "Image.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;
void ProjectionTest::run(int testType) {
    std::string outputDir = "../TestOutputs";
    if (!fs::exists(outputDir)) {
        fs::create_directories(outputDir);
    }

    Volume volume;
    if (!volume.loadVolume("../Scans/confuciusornis")) {
        std::cerr << "Failed to load volume from scans." << std::endl;
        return;
    }

    // Use a switch statement to decide which tests to run based on testType
    switch (testType) {
        case TestMIP:
            testMIP(volume, outputDir);
            break;
        case TestMINIP:
            testMINIP(volume, outputDir);
            break;
        case TestAIP:
            testAIP(volume, outputDir);
            break;
        default:
            std::cerr << "Unknown projection test type provided: " << testType << std::endl;
            break;
    }
}

void ProjectionTest::testMIP(const Volume& volume, const std::string& outputDir) {
    // Apply MIP projection and save the output
    Projection projection;
    std::string outputPath = outputDir + "/testVolume_MIP.png";
    projection.mip(volume, outputPath, 1, volume.getDepth());

    // Load the generated MIP image
    Image mipImage;
    if (!mipImage.loadImage(outputPath)) {
        std::cerr << "Failed to load MIP image for verification." << std::endl;
        return;
    }

    const auto& volumeData = volume.getData();
    int width = mipImage.getWidth();
    int height = mipImage.getHeight();
    int channels = mipImage.getChannels();
    const unsigned char* mipData = mipImage.getData();

    bool validMIP = true;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            unsigned char maxIntensity = 0;
            for (int z = 0; z < volume.getDepth(); ++z) {
                const auto& slice = volumeData[z];
                unsigned char sliceIntensity = slice[(y * width + x) * channels];
                if (sliceIntensity > maxIntensity) {
                    maxIntensity = sliceIntensity;
                }
            }

            unsigned char mipIntensity = mipData[(y * width + x) * channels];
            if (mipIntensity != maxIntensity) {
                validMIP = false;
                break;
            }
        }
        if (!validMIP) {
            break;
        }
    }

    if (validMIP) {
        std::cout << "MIP Test Passed: Maximum intensity matches expected values." << std::endl;
    } else {
        std::cerr << "MIP Test Failed: Discrepancy in maximum intensity values." << std::endl;
    }
}

void ProjectionTest::testMINIP(const Volume& volume, const std::string& outputDir) {
    // Apply MINIP projection and save the output
    Projection projection;
    std::string outputPath = outputDir + "/testVolume_MINIP.png";
    projection.minip(volume, outputPath, 1, volume.getDepth());

    // Load the generated MINIP image
    Image minipImage;
    if (!minipImage.loadImage(outputPath)) {
        std::cerr << "Failed to load MINIP image for verification." << std::endl;
        return;
    }

    const auto& volumeData = volume.getData();
    int width = minipImage.getWidth();
    int height = minipImage.getHeight();
    int channels = minipImage.getChannels();
    const unsigned char* minipData = minipImage.getData();

    bool validMINIP = true;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            unsigned char minIntensity = std::numeric_limits<unsigned char>::max();
            for (int z = 0; z < volume.getDepth(); ++z) {
                const auto& slice = volumeData[z];
                unsigned char sliceIntensity = slice[(y * width + x) * channels];
                if (sliceIntensity < minIntensity) {
                    minIntensity = sliceIntensity;
                }
            }

            unsigned char minipIntensity = minipData[(y * width + x) * channels];
            if (minipIntensity != minIntensity) {
                validMINIP = false;
                break;
            }
        }
        if (!validMINIP) {
            break;
        }
    }

    if (validMINIP) {
        std::cout << "MINIP Test Passed: Minimum intensity matches expected values." << std::endl;
    } else {
        std::cerr << "MINIP Test Failed: Discrepancy in minimum intensity values." << std::endl;
    }
}

void ProjectionTest::testAIP(const Volume& volume, const std::string& outputDir) {
    // Apply AIP projection and save the output
    Projection projection;
    std::string outputPath = outputDir + "/testVolume_AIP.png";
    projection.aip(volume, outputPath, 1, volume.getDepth());

    // Load the generated AIP image
    Image aipImage;
    if (!aipImage.loadImage(outputPath)) {
        std::cerr << "Failed to load AIP image for verification." << std::endl;
        return;
    }

    const auto& volumeData = volume.getData();
    int width = aipImage.getWidth();
    int height = aipImage.getHeight();
    int channels = aipImage.getChannels();
    const unsigned char* aipData = aipImage.getData();

    bool validAIP = true;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            unsigned long long totalIntensity = 0;
            for (int z = 0; z < volume.getDepth(); ++z) {
                const auto& slice = volumeData[z];
                totalIntensity += slice[(y * width + x) * channels];
            }
            unsigned char expectedAverage = static_cast<unsigned char>(totalIntensity / volume.getDepth());

            unsigned char aipIntensity = aipData[(y * width + x) * channels];
            if (aipIntensity != expectedAverage) {
                validAIP = false;
                break;
            }
        }
        if (!validAIP) {
            break;
        }
    }

    if (validAIP) {
        std::cout << "AIP Test Passed: Average intensity matches expected values." << std::endl;
    } else {
        std::cerr << "AIP Test Failed: Discrepancy in average intensity values." << std::endl;
    }
}