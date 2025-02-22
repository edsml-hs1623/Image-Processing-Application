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
 * @author acse-yw3523,edsml-lwk16, acse-ad2123, 
 *         edsml-hs1623, acse-xg1123, edsml-st2923,
 *         Group: selection sort.
 */

#ifndef FILTER_H
#define FILTER_H

#include "Image.h"

// The Filter class provides an interface for all image filters.
// Derived classes implement the apply method to apply the filter to an image.

class Filter {
public:
    // Virtual destructor for  cleanup of derived classes
    virtual ~Filter() = default;

    // Apply the filter to the given image
    virtual void apply(Image &image) = 0;
};

#endif // FILTER_H
