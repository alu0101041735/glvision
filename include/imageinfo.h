#ifndef IMAGEINFO_H
#define IMAGEINFO_H

#include <QString>
#include <QImage>

typedef std::pair<unsigned, unsigned> pair;

class imageInfo
{
private:
    QString fileType;
    pair dimensions;
    pair range;
    unsigned brightness;
    unsigned contrast;
    float entropy;


public:
    imageInfo();
    imageInfo(QString type, pair dim, pair range, unsigned br, unsigned cont, float ent);
    inline QString getFormat() {return fileType;}
    inline pair getDimensions() { return dimensions;}
    inline pair getRange() { return range;}
    inline unsigned getBrightness() { return brightness;}
    inline unsigned getContrast() { return contrast;}
    inline float getEntropy() { return entropy;}

};

#endif // IMAGEINFO_H
