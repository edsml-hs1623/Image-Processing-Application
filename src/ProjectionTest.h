#ifndef PROJECTIONTEST_H
#define PROJECTIONTEST_H

#include "Test.h"
#include "Volume.h"

enum TestType {
    TestMIP,
    TestMINIP,
    TestAIP,
    // Add more test types as necessary
};

class ProjectionTest : public Test {
public:
    // Updated run method to include testType parameter
    void run(int testType) override;

    // Other methods remain unchanged
    void testMIP(const Volume& volume, const std::string& outputDir);
    void testMINIP(const Volume& volume, const std::string& outputDir);
    void testAIP(const Volume& volume, const std::string& outputDir);
};

#endif // PROJECTIONTEST_H