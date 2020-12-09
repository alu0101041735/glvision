#include "imageinfo.h"

imageInfo::imageInfo()
{

}

imageInfo::imageInfo(QImage::Format type, pair dim, pair range, unsigned br, unsigned cont, unsigned ent) :
    fileType(type),
    dimensions(dim),
    range(range),
    brightness(br),
    contrast(cont),
    entropy(ent)
{

}
