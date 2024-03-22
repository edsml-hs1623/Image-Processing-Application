/**
 * @file Test.h
 * @author acse-yw3523,edsml-lwk16, acse-ad2123, 
 *         edsml-hs1623, acse-xg1123, edsml-st2923,
 *         Group: selection sort.
 */
#ifndef TEST_H
#define TEST_H

#include <string>

class Test {
public:
    // Make the destructor virtual in case we have dynamic allocation
    virtual ~Test() {}

    // Define the run method to take a TestType parameter with a default value of TestAll
    virtual void run(int testType) = 0;
};

#endif // TEST_H