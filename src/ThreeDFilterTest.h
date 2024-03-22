/**
 * @file ThreeDFilterTest.h
 * @author acse-yw3523,edsml-lwk16, acse-ad2123, 
 *         edsml-hs1623, acse-xg1123, edsml-st2923,
 *         Group: selection sort.
 */
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