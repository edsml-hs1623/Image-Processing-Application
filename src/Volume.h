#ifndef VOLUME_H
#define VOLUME_H

#include <string>
#include <vector>

class Volume {
public:
    Volume();
    ~Volume();

    bool loadVolume(const std::string& directoryPath);
    bool saveSlice(const std::string& filename, int sliceIndex, const std::string& format);

    int getWidth() const;
    int getHeight() const;
    int getDepth() const;

    std::vector<unsigned char> getSliceData(int sliceIndex) const;

private:
    void freeVolume();

    int width, height, depth, channels;
    std::vector<std::vector<unsigned char>> data;
};

#endif // VOLUME_H
