#ifndef NATIVEPROCESSOR_H
#define NATIVEPROCESSOR_H

#include <QImage>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <QDebug>



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
};

#endif // NATIVEPROCESSOR_H
