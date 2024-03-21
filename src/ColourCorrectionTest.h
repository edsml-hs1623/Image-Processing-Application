#ifndef COLOURCORRECTIONTEST_H
#define COLOURCORRECTIONTEST_H

#include "Test.h"

// Enum defining the types of tests that can be run.
enum ColourCorrectionTestType {
    TestGrayscale, // Test for grayscale conversion.
    TestBrightnessAdjustment, // Test for brightness adjustment.
    TestHistogramEqualization, // Test for histogram equalization.
    TestThresholding, // Test for thresholding.
    TestSaltAndPepperNoise // Test for adding salt and pepper noise.
};

class ColourCorrectionTest : public Test {
public:
    void run(int testType) override;

private:
    void testGrayscale(); // Tests conversion to grayscale.
    void testBrightnessAdjustment(); // Tests brightness adjustment.
    void testHistogramEqualization(); // Tests histogram equalization.
    void testThresholding(); // Tests thresholding.
    void testSaltAndPepperNoise(); // Tests adding salt and pepper noise.
};

#endif // COLOURCORRECTIONTEST_H