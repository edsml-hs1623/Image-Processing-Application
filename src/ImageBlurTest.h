/**
 * @file ImageBlurTest.h
 * @author acse-yw3523,edsml-lwk16, acse-ad2123, 
 *         edsml-hs1623, acse-xg1123, edsml-st2923,
 *         Group: selection sort.
 */
#ifndef IMAGEBLURTEST_H
#define IMAGEBLURTEST_H

#include "Test.h"
#include "Image.h"

// Enum defining the types of blur tests that can be run.
enum ImageBlurTestType {
    TestMedianBlur, // Test for Median Blur.
    TestBoxBlur, // Test for Box Blur.
    TestGaussianBlur // Test for Gaussian Blur.
};

class ImageBlurTest : public Test {
public:
    // Runs the specified blur test.
    void run(int testType) override;

private:
    void testMedianBlur(); // Tests the Median Blur method.
    void testBoxBlur(); // Tests the Box Blur method.
    void testGaussianBlur(); // Tests the Gaussian Blur method.
    double calculateStdDev(const Image& image); // Calculates standard deviation of the image.
    double calculateNoiseLevel(const Image& image); // Calculates noise level in the image.
};

#endif // IMAGEBLURTEST_H