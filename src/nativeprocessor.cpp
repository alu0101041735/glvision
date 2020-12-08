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

    new(&m_grayimage) QImage(width, height, QImage::Format_RGBA64);

   for (int i = 0; i < height; i++){
       for (int j = 0; j < width; j++) {
           red = m_image.pixelColor(j, i).red();
           green = m_image.pixelColor(j, i).green();
           blue = m_image.pixelColor(j, i).blue();

           result_color = (red + green + blue) / 3;

           color.setRed(result_color);
           color.setGreen(result_color);
           color.setBlue(result_color);

           m_grayimage.setPixelColor(j, i, color);
       }
   }

   return true;
}

void NativeProcessor::computeHistogram()
{
    new(&m_histogram) std::vector<uint16_t>(255,0);

    if (m_grayimage.isNull())
    {
        exit(-1);
    }

    int gray_level;

    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            gray_level = m_rimage.pixelColor(x, y).red();

            m_histogram[gray_level] += 1;
        }
    }


}

void NativeProcessor::computeCumulativeHistogram()
{
    if (m_histogram.size() == 0) {
        computeHistogram();
    }
    new(&m_cumulativehistogram) std::vector<uint16_t>(255,0);

    for (unsigned long i = 0; i < m_histogram.size(); i++) {
        m_cumulativehistogram[i] += m_histogram[i];
    }

}

void NativeProcessor::computeValueRange()
{
    std::pair<int, int> range;

    range.first = m_grayimage.pixelColor(0,0).red();
    range.second = m_grayimage.pixelColor(0,0).red();

    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            if (m_grayimage.pixelColor(x, y).red() < range.first)
               range.first = m_grayimage.pixelColor(x,y).red();
            if (m_grayimage.pixelColor(x, y).red() > range.second)
               range.second = m_grayimage.pixelColor(x,y).red();
        }
    }
    m_valuerange = range;

}

void NativeProcessor::computeBrightness()
{
    if (m_histogram.size() == 0)
        computeHistogram();

    int sum = 0;
    for (unsigned long i = 0; i < m_histogram.size(); i++) {

        for (int j = 0; j < m_histogram[i]; j++) {
            sum += i;
        }
    }

    m_brightness = sum / (m_width*m_height);

}

void NativeProcessor::computeContrast()
{
    if (m_histogram.size() == 0)
        computeHistogram();

    if (!m_brightness) {
        computeBrightness();
    }

    int sum = 0;

    for (unsigned long i = 0; i < m_histogram.size(); i++) {

        for (int j = 0; j < m_histogram[i]; j++) {
            int aux = i - m_brightness;
            sum += (aux*aux);
        }
    }

    m_contrast = sqrt(sum / (m_width*m_height));

}

NativeProcessor::NativeProcessor(QImage image): m_image(image)
{

    m_width = image.width();
    m_height = image.height();

    bool result = toGrayScale();
    computeHistogram();
    computeCumulativeHistogram();
    computeValueRange();
    computeGrayLevel();
    computeBrightness();
    computeContrast();


}

QImage NativeProcessor::processImage(int transformation)
{


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

void NativeProcessor::setImage()
{

}

int NativeProcessor::getWidth()
{
    return m_width;
}

int NativeProcessor::getHeight()
{
    return m_height;
}

std::vector<uint16_t> NativeProcessor::getHistogram()
{
    return m_histogram;
}

std::vector<uint16_t> NativeProcessor::getCumulativeHistogram()
{
    return m_cumulativehistogram;
}

std::pair<int, int> NativeProcessor::valueRange()
{
    return m_valuerange;
}

std::pair<QColor, int> NativeProcessor::grayLevel(int x, int y)
{
    std::pair<QColor, int> graylevel;

    graylevel.first = m_image.pixelColor(x, y);
    graylevel.second =  m_grayimage.pixelColor(x, y).red();

    return graylevel;
}

int NativeProcessor::brightness()
{
    return m_brightness;
}

int NativeProcessor::contrast()
{
    return m_contrast;
}
