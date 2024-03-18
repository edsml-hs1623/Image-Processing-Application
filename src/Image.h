// Image.h
#ifndef IMAGE_H // Start of the include guard
#define IMAGE_H

#include <string>


class Image {
public:
    Image();
    ~Image();
    bool loadImage(const std::string& filename);
    bool saveImage(const std::string& filename, const std::string& format);
    int getWidth() const;
    int getHeight() const;
    int getChannels() const;
    unsigned char* getData() const;

private:
    int width, height, channels;
    unsigned char* data;

    void freeImage();
};


#endif // IMAGE_H

