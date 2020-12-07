#include "nativeprocessor.h"

bool NativeProcessor::toGrayScale()
{
    int width = m_image.width();
    int height = m_image.height();
    QColor color;
    int red;
    int green;
    int blue;
    int result_color;

   for (int i = 0; i < height; i++){
       for (int j = 0; j < width; j++) {
           red = m_image.pixelColor(j, i).red();
           green = m_image.pixelColor(j, i).green();
           blue = m_image.pixelColor(j, i).blue();

           result_color = (red + green + blue) / 3;

           color.setRed(result_color);
           color.setGreen(result_color);
           color.setBlue(result_color);

           m_rimage.setPixelColor(j, i, color);
       }
   }

   return true;
}

NativeProcessor::NativeProcessor()
{

}

QImage NativeProcessor::processImage(QImage image, int transformation)
{
    int width = image.width();
    int height = image.height();

    m_image = image;
    new(&m_rimage) QImage(width, height, QImage::Format_RGBA64);

    switch(transformation) {

    case 1:

        break;
    case 2:
        if (!toGrayScale())
            exit(-1);
        break;
    case 4:
        break;
    case 8:
        break;

    }


    m_rimage.save("../glvision/images/test_cpu.png", "PNG");

    exit(0);
   return m_rimage;
}
