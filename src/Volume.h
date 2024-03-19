// Volume.h
#ifndef VOLUME_H
#define VOLUME_H

#include <string>
#include <vector>

class Volume {
public:
    Volume();
    ~Volume();

    bool loadVolume(const std::string& directoryPath);

    int getWidth() const;
    int getHeight() const;
    int getDepth() const;

    int getChannels() const;

    const std::vector<std::vector<unsigned char>>& getData() const;

private:
    void freeVolume();

    int width, height, depth, channels;
    std::vector<std::vector<unsigned char>> data; // Stores volume data
};

#endif // VOLUME_H
