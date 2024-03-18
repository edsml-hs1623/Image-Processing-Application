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
