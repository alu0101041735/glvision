#include "imageinfo.h"

imageInfo::imageInfo()
{

}

imageInfo::imageInfo(QString type, pair dim, pair range, unsigned br, unsigned cont, float ent) :
    fileType(type),
    dimensions(dim),
    range(range),
    brightness(br),
    contrast(cont),
    entropy(ent)
{

}
