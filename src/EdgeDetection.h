/**
 * @file EdgeDetection.h
 *
 * @brief Definition of the EdgeDetection class, a subclass of Filter, for edge detection in images.
 *
 * The EdgeDetection class provides a comprehensive solution for detecting edges in images. It
 * extends from the Filter base class and overrides the apply method to implement edge detection.
 * Supported edge detection methods include Sobel, Prewitt, Scharr, and Roberts Cross. This class
 * allows users to select and apply these methods to any image, facilitating the extraction of
 * structural information from images.
 *
 * The EdgeDetection class encapsulates different edge detection techniques and provides a unified
 * interface to apply these methods, making it easier to integrate and use in image processing pipelines.
 *
 * Key Features:
 *   - Supports multiple edge detection algorithms.
 *   - Inherits from the Filter class and overrides the apply method.
 *   - Offers a method to choose an edge detection algorithm based on user input.
 *
 * @note The Image class is required for applying edge detection methods. The effectiveness of each
 *       method may vary depending on the image and the specific requirements of the application.
 * @author acse-yw3523,edsml-lwk16, acse-ad2123, 
 *         edsml-hs1623, acse-xg1123, edsml-st2923,
 *         Group: selection sort.
 * Structure:
 *   EdgeDetection
 *   |-- prewitt
 *   |-- robertscross
 *   |-- scharr
 *   `-- sobel
 */

#ifndef EDGE_DETECTION_H
#define EDGE_DETECTION_H

#include "Filter.h"
#include "Image.h"
#include <iostream>
#include <vector>
#include <string>


enum EdgeOperator { Sobel, Prewitt, Scharr, RobertsCross };


class EdgeDetection : public Filter {
public:
    EdgeDetection(EdgeOperator operatorType);
    virtual ~EdgeDetection();

    void apply(Image& image) override;
    void applyEdge(Image& image, int choice);

    static EdgeOperator getEdgeOperatorFromChoice(int choice);

private:
    EdgeOperator operatorType;

    // Declare helper function
    static void applyEdgeDetection(Image& image, const std::vector<std::vector<int>>& kernelX, const std::vector<std::vector<int>>& kernelY);

    // Declare other private methods for individual edge detection algorithms
    void applySobel(Image& image);
    void applyPrewitt(Image& image);
    void applyScharr(Image& image);
    void applyRobertsCross(Image& image);
};

#endif // EDGE_DETECTION_H