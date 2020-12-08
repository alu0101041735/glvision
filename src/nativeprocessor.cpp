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

int NativeProcessor::getWidth(QImage image)
{
    return image.width();
}

int NativeProcessor::getHeight(QImage image)
{
    return image.height();
}

std::vector<uint16_t> NativeProcessor::getHistogram(QImage image)
{
    int width = image.width();
    int height = image.height();
    std::vector<uint16_t> histogram(255, 0);

    QImage rimage = processImage(image, 2);
    int gray_level;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            gray_level = rimage.pixelColor(x, y).red();

            histogram[gray_level] += 1;
        }
    }

    return histogram;
}

std::vector<uint16_t> NativeProcessor::getCumulativeHistogram(QImage image)
{
    int width = image.width();
    int height = image.height();
    std::vector<uint16_t> histogram= getHistogram(image);


    std::vector<uint16_t> cumulative_histogram(255, 0);

    for (unsigned long i = 0; i < histogram.size(); i++) {
        cumulative_histogram[i] += histogram[i];
    }

    return cumulative_histogram;

}

std::pair<int, int> NativeProcessor::valueRange(QImage image)
{
    QImage rimage = processImage(image, 2);
    std::pair<int, int> range;

    range.first = rimage.pixelColor(0,0).red();
    range.second = rimage.pixelColor(0,0).red();

    int height = image.height();
    int width = image.width();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (rimage.pixelColor(x, y).red() < range.first)
               range.first = rimage.pixelColor(x,y).red();
            if (rimage.pixelColor(x, y).red() > range.second)
               range.second = rimage.pixelColor(x,y).red();
        }
    }
    return range;
}

std::pair<QColor, int> NativeProcessor::grayLevel(QImage image, int x, int y)
{
    QImage rimage = processImage(image, 2);

    std::pair<QColor, int> graylevel;

    graylevel.first = image.pixelColor(x, y);
    graylevel.second =  rimage.pixelColor(x, y).red();

    return graylevel;
}
