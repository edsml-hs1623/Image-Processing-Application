#ifndef THREEFILTERTEST_H
#define THREEFILTERTEST_H

#include "Test.h"
#include "Volume.h"

enum FilterTestType {
    TestGaussian,
    TestMedian,
    // Add additional filter test types here if needed
};

class ThreeDFilterTest : public Test {
public:
    void run(int testType) override; // Updated signature
private:
    void testGaussianBlur();
    void testMedianBlur();
    double calculateStdDev(const Volume& volume);
};

#endif // THREEFILTERTEST_H