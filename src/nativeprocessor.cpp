#include "nativeprocessor.h"

void  NativeProcessor::toGrayScale()
{
    QColor color;
    float red;
    float green;
    float blue;
    int result_color;

    new(&m_grayimage) QImage(m_width, m_height, QImage::Format_RGBA64);

   for (int y = m_start.second; y < m_end.second; y++){
       for (int x = m_start.first; x < m_end.first; x++) {
           red = m_image.pixelColor(x, y).red();
           green = m_image.pixelColor(x, y).green();
           blue = m_image.pixelColor(x, y).blue();

           result_color = (red*0.222) + (green*0.707) + (blue*0.071);

           color.setRed(result_color);
           color.setGreen(result_color);
           color.setBlue(result_color);

           m_grayimage.setPixelColor(x, y, color);
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

    for (int y = m_start.second; y < m_end.second; y++) {
        for (int x = m_start.first; x < m_end.first; x++) {
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

void NativeProcessor::computeNormalizedHistogram()
{

    new(&m_normalizedhistogram) std::vector<double>(256,0.0);

    double h_size = m_histogram.size() -1;
    double size = m_height * m_width;

    double alpha = h_size / size;

    double aux = 0.0;
    for (unsigned long i = 0; i < m_histogram.size(); i++) {
        aux = m_histogram[i];
        m_normalizedhistogram[i] = aux * alpha;
    }
}

void NativeProcessor::computeNormalizedCumulativeHistogram()
{
    new(&m_normalizedcumulativehistogram) std::vector<double>(256,0.0);

    double h_size = m_cumulativehistogram.size() -1;
    double size = m_height * m_width;

    double alpha = h_size / size;
    double aux = 0.0;
    for (unsigned long i = 0; i < m_histogram.size(); i++) {
        aux = m_cumulativehistogram[i];
        m_normalizedcumulativehistogram[i] = aux * alpha;
    }

}

void NativeProcessor::computeValueRange()
{
    std::pair<int, int> range;

    range.first = m_grayimage.pixelColor(0,0).red();
    range.second = m_grayimage.pixelColor(0,0).red();

    for (int y = m_start.second; y < m_end.second; y++) {
        for (int x = m_start.first; x < m_end.first; x++) {
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
    float pi = 0.0;
    float normalized_histogram = 0.0;

    for (unsigned long i = 0; i < m_histogram.size(); i++) {
        if (m_histogram[i] != 0) {
            normalized_histogram = m_histogram[i];
            normalized_histogram /= (m_width*m_height);
            pi += normalized_histogram*log2(normalized_histogram);
        }

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

}

std::vector<std::pair<int, int>> NativeProcessor::computeFullStretch(std::pair<int, int> start, std::pair<int, int> end, std::pair<int, int> range)
{

    std::pair<float,float> aux_start = start;
    std::pair<float,float> aux_end = end;


    float slope = (aux_end.second - aux_start.second)/(aux_end.first - aux_start.first);
    float b = aux_start.second - (slope*aux_start.first);

    std::vector<std::pair<int, int>> result(range.second - range.first);

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

    resetZone();

    new(&m_rimage) QImage(m_width, m_height, QImage::Format_RGBA64);

    if (m_image.allGray()) {
        m_grayimage = m_image;
        m_isgrayscale = true;
    }
    else {
        toGrayScale();
    }

    computeHistogram();
    computeCumulativeHistogram();
    computeNormalizedHistogram();
    computeNormalizedCumulativeHistogram();
    computeEntropy();
    computeValueRange();
    computeBrightness();
    computeContrast();
}

NativeProcessor::NativeProcessor(QImage image, bool grayscale)
{
    m_width = image.width();
    m_height = image.height();


    resetZone();

    if (grayscale == true) {
        m_grayimage = image;
        m_isgrayscale = true;
    }
    else {
        toGrayScale();
    }

    computeHistogram();
    computeCumulativeHistogram();
    computeNormalizedHistogram();
    computeNormalizedCumulativeHistogram();
    computeEntropy();
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
        return m_grayimage;
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
    return m_grayimage.copy(m_start.first, m_start.second, m_end.first, m_end.second);
}

QImage NativeProcessor::getResultImage()
{
    return m_rimage.copy(m_start.first, m_start.second, m_end.first, m_end.second);
}

QImage NativeProcessor::getOriginalImage()
{
    m_image = m_image.copy(m_start.first, m_start.second, m_end.first, m_end.second);
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

std::vector<double> NativeProcessor::getNormalizedHistogram()
{
    return m_normalizedhistogram;
}

std::vector<double> NativeProcessor::getNormalizedCumulativeHistogram()
{
    return m_normalizedcumulativehistogram;
}

std::pair<int, int> NativeProcessor::valueRange()
{
    return m_valuerange;
}

float NativeProcessor::getEntropy()
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

QImage NativeProcessor::processStretch(std::vector<std::pair<int, int>> table)
{

    std::vector<std::pair<int, int>> fullstretch;


    for (uint16_t i = 1; i < table.size(); i++) {
        std::pair<int, int> start = table[i-1];
        std::pair<int, int> end = table[i];

        std::pair<int, int> size;
        size.first = start.first;
        size.second = end.first;

        //std::pair<int, int> *aux = computeFullStretch(start, end, size);
        std::vector<std::pair<int, int>> aux =  computeFullStretch(start, end, size);

        //int aux_size =  sizeof(&aux)/sizeof(std::pair<int, int>);

        //std::copy(aux, aux+aux_size, fullstretch[start.first]);
        int w = 0;
        fullstretch.insert(fullstretch.end(), aux.begin(), aux.end());
    }


    QColor updater;

    for (int y = m_start.second; y < m_end.second; y++) {
        for (int x = m_start.first; x < m_end.first; x++) {
            int color = m_grayimage.pixelColor(x, y).red();

            updater.setRgb(fullstretch[color].second,
                           fullstretch[color].second,
                           fullstretch[color].second);

            m_rimage.setPixelColor(x, y, updater);
        }
    }

    
    return m_rimage.copy(m_start.first, m_start.second, m_end.first, m_end.second);
}

QImage NativeProcessor::imageDifference(QImage image)
{
    NativeProcessor second(image);

    if ((m_width != second.getWidth()) || (m_height != second.getHeight())) {
        return m_image;
    }
    else {
        QColor colordiff;
    for (int y = m_start.second; y < m_end.second; y++) {
        for (int x = m_start.first; x < m_end.first; x++) {
                int diff = abs(second.grayLevel(x,y).second - m_grayimage.pixelColor(x,y).red());
                colordiff.setRgb(diff, diff, diff);
                m_rimage.setPixelColor(x, y, colordiff);
            }
        }
    }

    return m_rimage.copy(m_start.first, m_start.second, m_end.first, m_end.second);
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

    for (int y = m_start.second; y < m_end.second; y++) {
        for (int x = m_start.first; x < m_end.first; x++) {
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

    return m_rimage.copy(m_start.first, m_start.second, m_end.first, m_end.second);
}

QImage NativeProcessor::modifyContrast(float c)
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

    qDebug() << "INICIA\n\n";
    for (int y = m_start.second; y < m_end.second; y++) {
        for (int x = m_start.first; x < m_end.first; x++) {
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
    QImage rimage = m_rimage.copy(m_start.first, m_start.second, m_end.first, m_end.second);

    return  rimage;
}

QImage NativeProcessor::gammaCorrection(float gamma)
{

    float normalized_value;

    float red;
    float green;
    float blue;
    float newred;
    float newgreen;
    float  newblue;

    QColor result;

    for (int y = m_start.second; y < m_end.second; y++) {
        for (int x = m_start.first; x < m_end.first; x++) {
            red = m_image.pixelColor(x,y).red();
            green = m_image.pixelColor(x,y).green();
            blue = m_image.pixelColor(x,y).blue();


            normalized_value = red/255;
            newred = pow(normalized_value, gamma);
            normalized_value = green/255;
            newgreen = pow(normalized_value, gamma);
            normalized_value = blue/255;
            newblue = pow(normalized_value, gamma);

            result.setRgb(newred*255, newgreen*255, newblue*255);

            m_rimage.setPixelColor(x, y, result);
        }
    }

    return m_rimage.copy(m_start.first, m_start.second, m_end.first, m_end.second);
}

QImage NativeProcessor::gammaCorrectionGray(float gamma)
{

    float gray;
    float normalized_value;
    float b;

    QColor result;

    for (int y = m_start.second; y < m_end.second; y++) {
        for (int x = m_start.first; x < m_end.first; x++) {
            gray = m_grayimage.pixelColor(x,y).red();
            normalized_value = gray/255;
            b = pow(normalized_value, gamma);

            result.setRgb(b*255, b*255, b*255);

            m_rimage.setPixelColor(x, y, result);
        }
    }

    return m_rimage.copy(m_start.first, m_start.second, m_end.first, m_end.second);
}

QImage NativeProcessor::equalizeHistogram()
{
    std::vector<double> tul(256);
    float j;
    float aux;

    for (unsigned long i = 0; i < m_histogram.size(); i++) {
        j = i;
        float width = m_width;
        float height = m_height;
        aux = m_cumulativehistogram[j] * (255/(width*height));
        aux = round(aux);

        if (aux < 0)
            tul[i] = 0;
        else
            tul[i] = aux;
    }

    QColor color;
    int gray;

    for (int y = m_start.second; y < m_end.second; y++) {
        for (int x = m_start.first; x < m_end.first; x++) {
            gray = m_grayimage.pixelColor(x, y).red();

            color.setRgb(tul[gray], tul[gray], tul[gray]);

            m_rimage.setPixelColor(x, y, color);
        }
    }

    return m_rimage.copy(m_start.first, m_start.second, m_end.first, m_end.second);
}

QImage NativeProcessor::specifyHistogram(std::vector<double> otherhistogram)
{
    std::vector<int> tul(256,0.0);

    int j;
    for (uint16_t i = 0; i < m_histogram.size();  i++) {
        j = m_histogram.size() -1;
        do {
            tul[i] = j;
            j--;
        } while ((j > 0) && (m_normalizedcumulativehistogram[i] <= otherhistogram[j]));
    }

    int gray;
    QColor color;

    for (int y = m_start.second; y < m_end.second; y++) {
        for (int x = m_start.first; x < m_end.first; x++) {
            gray = m_grayimage.pixelColor(x, y).red();

            color.setRgb(tul[gray], tul[gray], tul[gray]);

            m_rimage.setPixelColor(x, y, color);
        }
    }

    return m_rimage.copy(m_start.first, m_start.second, m_end.first, m_end.second);
}

void NativeProcessor::updateImageInfo()
{
    computeHistogram();
    computeCumulativeHistogram();
    computeNormalizedHistogram();
    computeValueRange();
    computeEntropy();
    computeBrightness();
    computeContrast();
}

void NativeProcessor::setResultImageasGray()
{
    if (m_isgrayscale == true)
        m_grayimage = NativeProcessor(m_rimage, true).getGrayScale();
    else
        m_grayimage = NativeProcessor(m_rimage).getGrayScale();
}

void NativeProcessor::setGrayImageasOriginal()
{
    m_image = m_grayimage;
    m_isgrayscale = true;
}

void NativeProcessor::setResultImageasOriginal()
{
    if (m_isgrayscale == true)
        m_image = NativeProcessor(m_rimage, true).getOriginalImage();
    else
        m_image = NativeProcessor(m_rimage).getOriginalImage();
}

void NativeProcessor::setZone(std::pair<int, int> start, std::pair<int, int> end)
{
   m_start = start;
   m_end = end;
   new(&m_rimage) QImage(m_width, m_height, QImage::Format_RGBA64);

   updateImageInfo();
}

void NativeProcessor::resetZone()
{
   m_start.first = 0;
   m_start.second = 0;

   m_end.first = m_width;
   m_end.second = m_height;

    new(&m_rimage) QImage(m_width, m_height, QImage::Format_RGBA64);
}
