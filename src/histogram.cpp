#include "histogram.h"
#include <QPainter>

histogram::histogram(QVector<unsigned> histogramVector)
{
    QImage(256, 256*2, QImage::Format_ARGB32);
    QPainter painter(this);
    QPen pen;
    pen.setWidth(10);
    pen.setColor(Qt::black);
    painter.setPen(pen);
    unsigned pos = 0;
    for (auto value : histogramVector)
    {
        painter.drawLine(pos,0,pos,value);
        ++pos;
    }

}
