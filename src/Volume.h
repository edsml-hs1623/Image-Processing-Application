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

    std::vector<unsigned char> getData(int sliceIndex) const;
    void setVoxelValue(int z, int y, int x, unsigned char newValue);
    // Method to save a single slice as an image
    bool saveSliceAsImage(int sliceIndex, const std::string& filePath) const;

    // Method to save all slices in the volume as images
    bool saveVolumeAsImages(const std::string& outputDirectory) const;

private:
    void freeVolume();

    int width, height, depth, channels;
    std::vector<std::vector<unsigned char>> data;
};

#endif // VOLUME_H