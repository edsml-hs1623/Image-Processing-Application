/**
 * @file Volume.h
 * @author acse-yw3523,edsml-lwk16, acse-ad2123, 
 *         edsml-hs1623, acse-xg1123, edsml-st2923,
 *         Group: selection sort.
 */
#ifndef VOLUME_H
#define VOLUME_H

#include <string>
#include <vector>

class Volume {
public:
    Volume();
    ~Volume();

    void setData(const std::vector<std::vector<unsigned char>>& newData);

    bool loadVolume(const std::string& directoryPath);

    bool saveVolume(const std::string& directoryPath) const;


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