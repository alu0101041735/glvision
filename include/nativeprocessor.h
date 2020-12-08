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
#include <tgmath.h>


enum TransformationFlags
{
    GAUSSIAN  = 1,
    GRAYSCALE = 2,
    BLUR   = 4,
    TEST = 8
};


inline TransformationFlags operator|(TransformationFlags a, TransformationFlags b)
{
    return static_cast<TransformationFlags>(static_cast<int>(a) | static_cast<int>(b));
}


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

    int m_entropy;

    void toGrayScale();
    void toGaussian();

    void computeHistogram();
    void computeCumulativeHistogram();
    void computeValueRange();
    void computeEntropy();
    void computeBrightness();
    void computeContrast();

public:
    NativeProcessor(QImage image);
    QImage processImage( int transformation);
    void saveImage();
    int getWidth();
    int getHeight();
    QImage getGrayScale();
    std::vector<uint16_t> getHistogram();
    std::vector<uint16_t> getCumulativeHistogram();
    std::pair<int, int> valueRange();
    int getEntropy();
    std::pair<QColor, int> grayLevel(int x, int y);
    int brightness();
    int contrast();

};

#endif // NATIVEPROCESSOR_H
