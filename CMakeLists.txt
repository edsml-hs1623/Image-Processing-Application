cmake_minimum_required(VERSION 3.27)
project(advanced-programming-group-selection-sort)

set(CMAKE_CXX_STANDARD 17)

add_executable(advanced-programming-group-selection-sort
        src/stb_image.h
        src/stb_image_write.h
        src/ColourCorrection.cpp
        src/ColourCorrection.h
        src/EdgeDetection.cpp
        src/EdgeDetection.h
        src/Filter.h
        src/Image.cpp
        src/Image.h
        src/ImageBlur.cpp
        src/ImageBlur.h
        src/Projection.cpp
        src/Projection.h
        src/Slice.cpp
        src/Slice.h
        src/ThreeDFilter.cpp
        src/ThreeDFilter.h
        src/User_2D.cpp
        src/User_2D.h
        src/User_3D.cpp
        src/User_3D.cpp
        src/Volume.cpp
        src/Volume.h
        src/ColourCorrectionTest.cpp
        src/ColourCorrectionTest.h
        src/Test.h
        src/EdgeDetectionTest.cpp
        src/EdgeDetectionTest.h
        src/ImageBlurTest.cpp
        src/ImageBlurTest.h
        src/ProjectionTest.cpp
        src/ProjectionTest.h
        src/ThreeDFilterTest.cpp
        src/ThreeDFilterTest.h
        src/User_unitTests.cpp
        src/User_unitTests.h
        src/main.cpp)
