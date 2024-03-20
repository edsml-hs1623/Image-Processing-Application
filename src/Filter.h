/**
 * @file Filter.h
 *
 * @brief Abstract base class for image filtering operations.
 *
 * The Filter class serves as an interface for all image filters. It defines the
 * structure that each derived filter class should follow. The core functionality
 * of any filter is encapsulated in the apply method, which each subclass must implement.
 * This approach allows for a variety of filters to be applied to images, making the
 * system extensible and flexible for various image processing tasks.
 *
 * Usage:
 *   class CustomFilter : public Filter {
 *   public:
 *       void apply(Image &image) override {
 *           // Custom filter implementation
 *       }
 *   };
 *
 *   Image img;
 *   CustomFilter filter;
 *   filter.apply(img); // Applies the custom filter to img
 *
 * @note The Image class should be included and properly defined for this interface to work.
 *       Derived filter classes must implement the apply method for specific filtering operations.
 *
 * @author [Your Name or Your Organization]
 * @version [Version Number]
 * @date [Date of Last Update]
 */

#ifndef FILTER_H
#define FILTER_H

#include "Image.h"


// The Filter class provides an interface for all image filters.
// Derived classes should implement the apply method to apply the filter to an image.

class Filter {
public:
    // Virtual destructor to ensure proper cleanup of derived classes
    virtual ~Filter() = default;

    // Apply the filter to the given image
    virtual void apply(Image &image) = 0;
};

#endif // FILTER_H