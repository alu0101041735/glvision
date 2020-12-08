#ifndef GLIMAGE_H
#define GLIMAGE_H

#include <QImage>
#include <QObject>
#include <QVector>

namespace GlVision {

class GLImage : public QImage
{
public:
    GLImage() {}
    GLImage(QImage image) : QImage(image) {}
    GLImage(QString path) : QImage(path) {}

private:

public:
    QVector<unsigned> histogram();


};
}

#endif // GLIMAGE_H
