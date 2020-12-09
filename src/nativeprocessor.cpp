#include "nativeprocessor.h"

void  NativeProcessor::toGrayScale()
{
    QColor color;
    int red;
    int green;
    int blue;
    int result_color;

    new(&m_grayimage) QImage(m_width, m_height, QImage::Format_RGBA64);

   for (int i = 0; i < m_height; i++){
       for (int j = 0; j < m_width; j++) {
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

}

void NativeProcessor::computeHistogram()
{
    new(&m_histogram) std::vector<uint32_t>(256,0);

    if (m_grayimage.isNull())
    {
        exit(-1);
    }

    int gray_level;

    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            gray_level = m_grayimage.pixelColor(x, y).red();

            m_histogram[gray_level] += 1;
        }
    }


}

void NativeProcessor::computeCumulativeHistogram()
{
    if (m_histogram.size() == 0) {
        computeHistogram();
    }
    new(&m_cumulativehistogram) std::vector<uint32_t>(256,0);

    uint32_t sum = 0;;
    for (unsigned long i = 0; i < m_histogram.size(); i++) {
        sum += m_histogram[i];
        m_cumulativehistogram[i] = sum;
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

void NativeProcessor::computeEntropy()
{
    long double pi = 0;
    float aux;

    for (unsigned long i = 0; i < m_histogram.size(); i++) {
        aux = (i/m_histogram.size());
        pi += aux*log2(aux);
    }

    m_entropy = -pi;
}

void NativeProcessor::computeBrightness()
{
    if (m_histogram.size() == 0)
        computeHistogram();

    int sum = 0;
    for (unsigned long i = 0; i < m_histogram.size(); i++) {
        for (unsigned long int j = 0; j < m_histogram[i]; j++) {
            sum += i;
        }
    }

    m_brightness = sum / (m_width*m_height);

}

void NativeProcessor::computeContrast()
{
    if (m_histogram.size() == 0)
        computeHistogram();

    uint64_t sum = 0;


    for (unsigned long i = 0; i < m_histogram.size(); i++) {

        for (unsigned long int j = 0; j < m_histogram[i]; j++) {
            long double aux = abs(pow(i - m_brightness, 2));
            sum += aux;
        }
    }

    m_contrast = sqrt(sum / (m_width*m_height));
    std::cout << "CONTRASTE: " << sum << " " << m_contrast << "\n\n\n\n\n";

}

std::pair<int, int> *NativeProcessor::computeFullStretch(std::pair<int, int> start, std::pair<int, int> end, std::pair<int, int> range)
{

    float slope = (end.second - start.second)/(end.first - start.first);
    float b = start.second - slope*start.first;

    std::pair<int, int> *result = new std::pair<int, int>(range.first - range.second, 0);

    int v = 0;
    for (int i = range.first; i < range.second; i++) {
       result[v].first = i;
       result[v].second =  slope*i + b;
       v++;
    }

    return result;
}

NativeProcessor::NativeProcessor(QImage image): m_image(image)
{

    m_width = image.width();
    m_height = image.height();

    new(&m_rimage) QImage(m_width, m_height, QImage::Format_RGBA64);

    toGrayScale();
    computeHistogram();
    computeCumulativeHistogram();
    computeValueRange();
    computeBrightness();
    computeContrast();
}

QImage NativeProcessor::processImage(int transformation)
{


    switch(transformation) {

    case 1:

        break;
    case 2:
        toGrayScale();
        break;
    case 4:
        break;
    case 8:
        break;
    default:
        m_rimage = m_grayimage;
        break;

    }
    return m_rimage;
}

void NativeProcessor::saveImage()
{
    m_rimage.save("../glvision/images/test.png", "PNG");

}

int NativeProcessor::getWidth()
{
    return m_width;
}

int NativeProcessor::getHeight()
{
    return m_height;
}

QImage NativeProcessor::getGrayScale()
{
    return m_grayimage;
}

QImage NativeProcessor::getResultImage()
{
    return m_rimage;
}

QImage NativeProcessor::getOriginalImage()
{
    return m_image;
}

std::vector<uint32_t> NativeProcessor::getHistogram()
{
    return m_histogram;
}

std::vector<uint32_t> NativeProcessor::getCumulativeHistogram()
{
    return m_cumulativehistogram;
}

std::pair<int, int> NativeProcessor::valueRange()
{
    return m_valuerange;
}

int NativeProcessor::getEntropy()
{
    return m_entropy;
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

QImage NativeProcessor::processStretch(std::pair<int, int> *table)
{

    std::pair<int, int> fullstretch[256];

    int size = sizeof(&table)/sizeof(std::pair<int, int>);

    for (int i = 1; i < size; i++) {
        std::pair<int, int> start = table[i-1];
        std::pair<int, int> end = table[i];

        std::pair<int, int> size;
        size.first = start.first;
        size.second = end.first;

        std::pair<int, int> *aux = computeFullStretch(start, end, size);

        int aux_size =  sizeof(&aux)/sizeof(std::pair<int, int>);

        //std::copy(aux, aux+aux_size, fullstretch[start.first]);
        int w = 0;
        for (int i = start.first; i < end.first; i++) {
            fullstretch[i].first = aux[w].first;
            fullstretch[i].second = aux[w].second;
            w++;
        }
    }

    size = sizeof(fullstretch)/sizeof(std::pair<int, int>);

    QColor updater;

    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            int color = m_grayimage.pixelColor(x, y).red();

            updater.setRgb(fullstretch[color].second,
                           fullstretch[color].second,
                           fullstretch[color].second);

            m_rimage.setPixelColor(x, y, updater);
        }
    }
    return m_rimage;
}

QImage NativeProcessor::imageDifference(QImage image)
{
    NativeProcessor second(image);

    if ((m_width != second.getWidth()) || (m_height != second.getHeight())) {
       std::cout << "Fuc u!\n";
    }
    else {
        QColor colordiff;
        for (int y = 0; y < m_height; y++) {
            for (int x = 0; x < m_width; x++) {
                int diff = abs(second.grayLevel(x,y).second - m_grayimage.pixelColor(x,y).red());
                colordiff.setRgb(diff, diff, diff);
                m_rimage.setPixelColor(x, y, colordiff);
            }
        }
    }
    return m_rimage;
}

QImage NativeProcessor::modifyBrightness(float br)
{
    if (br < 0)
        br = -br;
    QColor newcolor;

    int red;
    int green;
    int blue;

    int new_red;
    int new_green;
    int new_blue;

    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            red = m_image.pixelColor(x, y).red();
            green = m_image.pixelColor(x, y).green();
            blue = m_image.pixelColor(x, y).blue();

            new_red = red * br;
            new_green = green * br;
            new_blue = blue * br;

            new_red = new_red < 0 ? 0 : new_red;
            new_red = new_red > 255 ? 255 : new_red;

            new_green = new_green < 0 ? 0 : new_green;
            new_green = new_green > 255 ? 255 : new_green;

            new_blue = new_blue < 0 ? 0 : new_blue;
            new_blue = new_blue > 255 ? 255 : new_blue;

            newcolor.setRgb(new_red, new_green, new_blue);

            m_rimage.setPixelColor(x, y, newcolor);
        }
    }
    return m_rimage;
}

QImage NativeProcessor::modifyContrast(int c)
{
    if ( c > 255)
        c = 255;
    if (c < -255)
        c = -255;
    int red;
    int green;
    int blue;
    int new_red;
    int new_green;
    int new_blue;
    float fcf = (259*(c + 255))/(255*(259 - c));

    QColor newcolor;

        for (int y = 0; y < m_height; y++) {
            for (int x = 0; x < m_width; x++) {
                red = m_image.pixelColor(x, y).red();
                green = m_image.pixelColor(x, y).green();
                blue = m_image.pixelColor(x, y).blue();

                new_red = (fcf * (red - 128)) + 128;
                new_green = (fcf * (green - 128)) + 128;
                new_blue = (fcf * (blue - 128)) + 128;

                new_red = new_red < 0 ? 0 : new_red;
                new_red = new_red > 255 ? 255 : new_red;

                new_green = new_green < 0 ? 0 : new_green;
                new_green = new_green > 255 ? 255 : new_green;

                new_blue = new_blue < 0 ? 0 : new_blue;
                new_blue = new_blue > 255 ? 255 : new_blue;

                newcolor.setRgb(new_red, new_green, new_blue);

                m_rimage.setPixelColor(x, y, newcolor);
            }
        }
        return m_rimage;
}

void NativeProcessor::updateImageInfo()
{
    computeHistogram();
    computeCumulativeHistogram();
    computeValueRange();
    computeEntropy();
    computeBrightness();
    computeContrast();
}

void NativeProcessor::setResultImageasGray()
{
    m_grayimage = NativeProcessor(m_rimage).getGrayScale();
}

void NativeProcessor::setGrayImageasOriginal()
{
    m_image = NativeProcessor(m_grayimage).getOriginalImage();
}

void NativeProcessor::setResultImageasOriginal()
{
    m_image = NativeProcessor(m_rimage).getOriginalImage();
}
