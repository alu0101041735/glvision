#include "glimage.h"
#include <QDebug>

QVector<unsigned> GlVision::GLImage::histogram()
{
    //QImage image = convertToFormat(QImage::Format_ARGB32);

    QVector<unsigned> histogram;
    histogram.fill(0,256);
    qDebug() << histogram.size();
    for (int row = 0; row < height(); ++row)
    {
        QRgb* line = (QRgb*)scanLine(row);
        for (int col = 0; col < width(); ++col)
        {
            QRgb pixel = *line;
            ++line;
            unsigned grayPixel = qGray(pixel);
            unsigned grayConverted = qGray(pixel);
            histogram[grayConverted] += 1;
        }

    }

    return histogram;
}
