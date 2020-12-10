#ifndef IMAGEINFO_H
#define IMAGEINFO_H

#include <QString>
#include <QPair>
#include <QImage>

typedef QPair<unsigned, unsigned> pair;

class imageInfo
{
private:
    QString fileType;
    pair dimensions;
    pair range;
    unsigned brightness;
    unsigned contrast;
    unsigned entropy;


public:
    imageInfo();
    imageInfo(QString type, pair dim, pair range, unsigned br, unsigned cont, unsigned ent);
    inline QString getFormat() {return fileType;}
    inline pair getDimensions() { return dimensions;}
    inline pair getRange() { return range;}
    inline unsigned getBrightness() { return brightness;}
    inline unsigned getContrast() { return contrast;}
    inline unsigned getEntropy() { return entropy;}

};

#endif // IMAGEINFO_H
