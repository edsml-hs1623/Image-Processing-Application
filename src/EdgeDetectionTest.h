#ifndef EDGE_DETECTION_TEST_H
#define EDGE_DETECTION_TEST_H

#include "Test.h"
#include "Image.h"
#include "EdgeDetection.h"

// Enum defining the types of edge detection tests that can be run.
enum EdgeDetectionTestType {
    TestSobel, // Test for Sobel edge detection.
    TestPrewitt, // Test for Prewitt edge detection.
    TestScharr, // Test for Scharr edge detection.
    TestRobertsCross // Test for Roberts Cross edge detection.
};

class EdgeDetectionTest : public Test {
public:
    // Runs the specified edge detection test.
    void run(int testType) override;

private:
    void testSobel(); // Tests Sobel edge detection method.
    void testPrewitt(); // Tests Prewitt edge detection method.
    void testScharr(); // Tests Scharr edge detection method.
    void testRobertsCross(); // Tests Roberts Cross edge detection method.
};

#endif // EDGE_DETECTION_TEST_H