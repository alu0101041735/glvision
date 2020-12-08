#ifndef NATIVEPROCESSOR_H
#define NATIVEPROCESSOR_H

#include <QImage>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <QDebug>
#include <unordered_map>
#include <utility>
#include <set>


class NativeProcessor
{
private:
    QImage m_image;
    QImage m_rimage;

    bool toGrayScale();
    bool toGaussian();

public:
    NativeProcessor();
    QImage processImage(QImage image, int transformation);
    QImage processImage(int transformation);
    void setImage(QImage image);
    int getWidth(QImage image);
    int getHeight(QImage image);
    std::vector<uint16_t> getHistogram(QImage image);
    std::vector<uint16_t> getCumulativeHistogram(QImage image);
    std::pair<int, int> valueRange(QImage image);

};

#endif // NATIVEPROCESSOR_H
