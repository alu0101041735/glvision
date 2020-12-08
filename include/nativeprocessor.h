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
#include <math.h>

class NativeProcessor
{
private:
    QImage m_image;
    QImage m_grayimage;
    QImage m_rimage;

    int m_width;
    int m_height;

    std::vector<uint16_t> m_histogram;
    std::vector<uint16_t> m_cumulativehistogram;

    std::pair<int, int> m_valuerange;

    int m_brightness;
    int m_contrast;

    bool toGrayScale();
    bool toGaussian();

    void computeHistogram();
    void computeCumulativeHistogram();
    void computeValueRange();
    void computeBrightness();
    void computeContrast();

public:
    NativeProcessor(QImage image);
    QImage processImage( int transformation);
    void setImage();
    int getWidth();
    int getHeight();
    std::vector<uint16_t> getHistogram();
    std::vector<uint16_t> getCumulativeHistogram();
    std::pair<int, int> valueRange();
    std::pair<QColor, int> grayLevel(int x, int y);
    int brightness();
    int contrast();

};

#endif // NATIVEPROCESSOR_H
