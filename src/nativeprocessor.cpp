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

        long double aux = m_histogram[i]*abs(pow(i - m_brightness, 2));
        sum += aux;
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

void NativeProcessor::createLUT(bool sum, float factor, int size)
{
    new (&m_lut) std::vector<int>(size);

    if (sum) {
        for (int i = 0; i < size; i++) {
            m_lut[i] = (float)i + factor;
        }
    }
    else {
        for (int i = 0; i < size; i++) {
            m_lut[i] = factor * ((float)i - 128) + 128;
        }
    }
}

float NativeProcessor::min(float a, float b, float c, float d)
{
   float aux1 = std::min(a,b);
   float aux2 = std::min(c,d);

   return std::min(aux1,aux2);
}

float NativeProcessor::max(float a, float b, float c, float d)
{
   float aux1 = std::max(a,b);
   float aux2 = std::max(c,d);

   return std::max(aux1,aux2);
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
    return m_grayimage.copy(m_rect);
}

QImage NativeProcessor::getResultImage()
{
    return m_rimage.copy(m_rect);
}

QImage NativeProcessor::getOriginalImage()
{
    return m_image.copy(m_rect);
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

    
    return m_rimage.copy(m_rect);
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

    return m_rimage.copy(m_rect);
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

    float B = (float)br - (float)m_brightness;
    createLUT(true, B, m_histogram.size());

    for (int y = m_start.second; y < m_end.second; y++) {
        for (int x = m_start.first; x < m_end.first; x++) {
            red = m_image.pixelColor(x, y).red();
            green = m_image.pixelColor(x, y).green();
            blue = m_image.pixelColor(x, y).blue();

            new_red = m_lut[red];
            new_green = m_lut[green];
            new_blue = m_lut[blue];

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

    return m_rimage.copy(m_rect);
}

QImage NativeProcessor::modifyContrast(float c)
{
    int red;
    int green;
    int blue;
    int new_red;
    int new_green;
    int new_blue;

    float A = (float)c / (float)m_contrast;
    createLUT(false, A, m_histogram.size());

    QColor newcolor;

    for (int y = m_start.second; y < m_end.second; y++) {
        for (int x = m_start.first; x < m_end.first; x++) {
                red = m_image.pixelColor(x, y).red();
                green = m_image.pixelColor(x, y).green();
                blue = m_image.pixelColor(x, y).blue();

                new_red = m_lut[red];
                new_green = m_lut[green];
                new_blue = m_lut[blue];

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

    return  m_rimage.copy(m_rect);
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

    return m_rimage.copy(m_rect);
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

    return m_rimage.copy(m_rect);
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

    return m_rimage.copy(m_rect);
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

    return m_rimage.copy(m_rect);
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
   m_start.second -=1;
   m_end.second -=1;
   new(&m_rimage) QImage(m_width, m_height, QImage::Format_RGBA64);
    m_rect = QRect(QPoint(m_start.first, m_start.second), QPoint(m_end.first, m_end.second));

   updateImageInfo();
}

void NativeProcessor::resetZone()
{
   m_start.first = 0;
   m_start.second = 0;

   m_end.first = m_width-1;
   m_end.second = m_height-1;

    m_rect = QRect(QPoint(m_start.first, m_start.second), QPoint(m_end.first, m_end.second));

   new(&m_rimage) QImage(m_width, m_height, QImage::Format_RGBA64);
}

QImage NativeProcessor::vMirror()
{
    int yAux = m_end.second;

    int red;
    int green;
    int blue;
    QColor aux;

    for (int y = m_start.second; y < m_end.second; y++) {
        for (int x = m_start.first; x < m_end.first; x++) {
            red = m_image.pixelColor(x,yAux).red();
            green = m_image.pixelColor(x,yAux).green();
            blue = m_image.pixelColor(x,yAux).blue();

            aux.setRed(red);
            aux.setGreen(green);
            aux.setBlue(blue);

            m_rimage.setPixelColor(x, y, aux);

        }
        yAux--;
    }

    return m_rimage.copy(m_rect);
}

QImage NativeProcessor::hMirror()
{
    int xAux;

    int red;
    int green;
    int blue;
    QColor aux;

    for (int y = m_start.second; y < m_end.second; y++) {
        xAux = m_end.second;
        for (int x = m_start.first; x < m_end.first; x++) {
            red = m_image.pixelColor(xAux,y).red();
            green = m_image.pixelColor(xAux,y).green();
            blue = m_image.pixelColor(xAux,y).blue();

            aux.setRed(red);
            aux.setGreen(green);
            aux.setBlue(blue);

            m_rimage.setPixelColor(x, y, aux);

            xAux--;
        }
    }

    return m_rimage.copy(m_rect);
}

QImage NativeProcessor::transposed()
{
    int red;
    int green;
    int blue;
    QColor aux;

    new(&m_rimage) QImage(m_height, m_width, QImage::Format_RGBA64);

    for (int y = m_start.second; y < m_end.second; y++) {
        for (int x = m_start.first; x < m_end.first; x++) {
            red = m_image.pixelColor(x,y).red();
            green = m_image.pixelColor(x,y).green();
            blue = m_image.pixelColor(x,y).blue();

            aux.setRed(red);
            aux.setGreen(green);
            aux.setBlue(blue);

            m_rimage.setPixelColor(y, x, aux);
        }
    }

    return m_rimage.copy(m_rect);
}

QImage NativeProcessor::basicRotation(int r)
{
    int red;
    int green;
    int blue;
    QColor aux;
    qDebug() << r;

    if (r == 90) {
        new(&m_rimage) QImage(m_end.second-m_start.second, m_end.first-m_start.first, QImage::Format_RGBA64);

        int yAux = m_end.first;

        for (int y = 0; y < m_rimage.height(); y++) {
            for (int x = 0;  x < m_rimage.width(); x++) {
                red = m_image.pixelColor(x, y).red();
                green = m_image.pixelColor(x, y).green();
                blue = m_image.pixelColor(x, y).blue();

                aux.setRed(red);
                aux.setGreen(green);
                aux.setBlue(blue);

                //datOut[i][j]= datIn[rows-j][i];
                m_rimage.setPixelColor(m_end.second-y, x, aux);

            }
            yAux--;
        }
        return m_rimage;
        //return m_rimage.copy(m_rect);
    }
    else if (r == 180) {
        new(&m_rimage) QImage(m_end.first-m_start.first, m_end.second-m_start.first, QImage::Format_RGBA64);

        for (int y = 0; y < m_rimage.height(); y++) {
            for (int x = 0;  x < m_rimage.width(); x++) {
                red = m_image.pixelColor(x, y).red();
                green = m_image.pixelColor(x, y).green();
                blue = m_image.pixelColor(x, y).blue();


                aux.setRed(red);
                aux.setGreen(green);
                aux.setBlue(blue);

                m_rimage.setPixelColor(m_rimage.width()-x,m_rimage.height()-y, aux);
            }
        }

        return m_rimage.copy(m_rect);
    }
    else if (r == 270) {
        new(&m_rimage) QImage(m_end.second-m_start.first, m_end.first-m_start.first, QImage::Format_RGBA64);

        int xAux;

        for (int y = m_start.second; y < m_end.second; y++) {
            xAux = m_end.second;
            for (int x = m_start.first;  x < m_end.first; x++) {
                red = m_image.pixelColor(y, xAux).red();
                green = m_image.pixelColor(y, xAux).green();
                blue = m_image.pixelColor(y, xAux).blue();

                aux.setRed(red);
                aux.setGreen(green);
                aux.setBlue(blue);

                m_rimage.setPixelColor(x, y, aux);

                xAux--;
            }
        }
        return m_rimage;
    }
    else {
        return m_image;
    }
}

QImage NativeProcessor::rotateWrong(int r)
{
    double pi = 3.14159265;

    int red;
    int green;
    int blue;

    QColor aux;

    float yAux;
    float xAux;

    std::pair<int, int> center;
    center.first = ((m_end.first-m_start.first)/(2));
    center.second = ((m_end.second-m_start.second)/(2));
    float angle1;
    float finalAngle;

    for (int y = m_start.second; y < m_end.second; y++) {
        for (int x = m_start.first;  x < m_end.first; x++) {
            red = m_image.pixelColor(x, y).red();
            green = m_image.pixelColor(x, y).green();
            blue = m_image.pixelColor(x, y).blue();

            int relativeX = x - center.first;
            int relativeY = y - center.second;

            float slope = ((float)relativeY-(float)center.second) / ((float)relativeX-(float)center.first);
            angle1 = atan(slope*(pi/180));

            finalAngle = angle1 - r;

            xAux = (cos(finalAngle * (pi/180))*relativeX) - (sin(finalAngle * (pi/180))*relativeY);
            yAux = (sin(finalAngle * (pi/180))*relativeX) + (cos(finalAngle * (pi/180))*relativeY);

            xAux += center.first;
            yAux += center.second;


            aux.setRed(red);
            aux.setGreen(green);
            aux.setBlue(blue);

            m_rimage.setPixelColor(xAux, yAux, aux);
        }
    }

    return m_rimage;
}

ImageWithoutCorners NativeProcessor::rotateVMP(int r)
{
    double pi = 3.14159265;

    std::pair<float,float> cornerA;
    cornerA.first = (cos(r * (pi/180))*m_start.first) - (sin(r * (pi/180))*m_start.second);
    cornerA.second = (sin(r * (pi/180))*m_start.first) + (cos(r * (pi/180))*m_start.second);

    std::pair<float,float> cornerB;
    cornerB.first = (cos(r * (pi/180))*m_end.first) - (sin(r * (pi/180))*m_start.second);
    cornerB.second = (sin(r * (pi/180))*m_end.first) + (cos(r * (pi/180))*m_start.second);

    std::pair<float,float> cornerC;
    cornerC.first = (cos(r * (pi/180))*m_start.first) - (sin(r * (pi/180))*m_end.second);
    cornerC.second = (sin(r * (pi/180))*m_start.first) + (cos(r * (pi/180))*m_end.second);

    std::pair<float,float> cornerD;
    cornerD.first = (cos(r * (pi/180))*m_end.first) - (sin(r * (pi/180))*m_end.second);
    cornerD.second = (sin(r * (pi/180))*m_end.first) + (cos(r * (pi/180))*m_end.second);

    float minW = min(cornerA.first, cornerB.first, cornerC.first, cornerD.first);
    float maxW = max(cornerA.first, cornerB.first, cornerC.first, cornerD.first);

    float minH = min(cornerA.second, cornerB.second, cornerC.second, cornerD.second);
    float maxH = max(cornerA.second, cornerB.second, cornerC.second, cornerD.second);

    int newHeight = maxH - minH;
    int newWidth = maxW - minW;

    new(&m_rimage) QImage(newWidth, newHeight,QImage::Format_RGBA64);

    int inverseR = -r;

    std::pair<float, float> originCoord;
    QColor auxColor;
    long int addedBlack = 0;

    for (int y = 0; y < m_rimage.height(); y++) {
        for (int x = 0; x < m_rimage.width(); x++) {

            std::pair<float, float> coord;

            coord.first = x + minW;;
            coord.second = y + minH;

            originCoord.first = (cos(inverseR * (pi/180))*coord.first) - (sin(inverseR * (pi/180))*coord.second);
            originCoord.second = (sin(inverseR * (pi/180))*coord.first) + (cos(inverseR * (pi/180))*coord.second);

            if ((originCoord.first >= 0) && (originCoord.first < m_image.width())
                    && (originCoord.second >= 0) && (originCoord.second < m_image.height())) {

                auxColor = m_image.pixelColor(originCoord.first, originCoord.second);

                m_rimage.setPixelColor(x,y, auxColor);
            } else {
                m_rimage.setPixelColor(x,y,QColor(0,0,0));
                addedBlack++;
            }
        }
    }
    m_imageWithoutCorners.image = m_rimage;
    m_imageWithoutCorners.subNumber = addedBlack;

    return m_imageWithoutCorners;
}

ImageWithoutCorners NativeProcessor::rotateBilineal(int r)
{
    double pi = 3.14159265;

    std::pair<float,float> cornerA;
    cornerA.first = (cos(r * (pi/180))*m_start.first) - (sin(r * (pi/180))*m_start.second);
    cornerA.second = (sin(r * (pi/180))*m_start.first) + (cos(r * (pi/180))*m_start.second);

    std::pair<float,float> cornerB;
    cornerB.first = (cos(r * (pi/180))*m_end.first) - (sin(r * (pi/180))*m_start.second);
    cornerB.second = (sin(r * (pi/180))*m_end.first) + (cos(r * (pi/180))*m_start.second);

    std::pair<float,float> cornerC;
    cornerC.first = (cos(r * (pi/180))*m_start.first) - (sin(r * (pi/180))*m_end.second);
    cornerC.second = (sin(r * (pi/180))*m_start.first) + (cos(r * (pi/180))*m_end.second);

    std::pair<float,float> cornerD;
    cornerD.first = (cos(r * (pi/180))*m_end.first) - (sin(r * (pi/180))*m_end.second);
    cornerD.second = (sin(r * (pi/180))*m_end.first) + (cos(r * (pi/180))*m_end.second);

    float minW = min(cornerA.first, cornerB.first, cornerC.first, cornerD.first);
    float maxW = max(cornerA.first, cornerB.first, cornerC.first, cornerD.first);

    float minH = min(cornerA.second, cornerB.second, cornerC.second, cornerD.second);
    float maxH = max(cornerA.second, cornerB.second, cornerC.second, cornerD.second);

    int newHeight = maxH - minH;
    int newWidth = maxW - minW;

    new(&m_rimage) QImage(newWidth, newHeight,QImage::Format_RGBA64);

    int inverseR = -r;

    std::pair<float, float> originCoord;
    QColor auxColor;
    int addedBlack = 0;

    for (int y = 0; y < m_rimage.height(); y++) {
        for (int x = 0; x < m_rimage.width(); x++) {

            std::pair<float, float> coord;

            coord.first = x + minW;;
            coord.second = y + minH;

            originCoord.first = (cos(inverseR * (pi/180))*coord.first) - (sin(inverseR * (pi/180))*coord.second);
            originCoord.second = (sin(inverseR * (pi/180))*coord.first) + (cos(inverseR * (pi/180))*coord.second);

            if ((originCoord.first >= 0) && (originCoord.first < m_image.width())
                    && (originCoord.second >= 0) && (originCoord.second < m_image.height())) {

                auxColor = m_image.pixelColor(originCoord.first, originCoord.second);

                m_rimage.setPixelColor(x,y, auxColor);
            } else {
                m_rimage.setPixelColor(x,y,QColor(0,0,0));
                addedBlack++;
            }
        }
    }
    m_imageWithoutCorners.image = m_rimage;
    m_imageWithoutCorners.subNumber = addedBlack;

    return m_imageWithoutCorners;
}


QImage NativeProcessor::scale(float xScale, float yScale)
{
    QImage auxImage = m_image.copy(m_rect);

    int newHeight = (float)auxImage.height() * yScale;
    int newWidth = (float)auxImage.width() * xScale;

    new(&m_rimage) QImage(newWidth, newHeight,QImage::Format_RGBA64);
    int red;
    int green;
    int blue;
    QColor aux;

    for (int y = 0; y < m_rimage.height(); y++) {
        for (int x = 0; x < m_rimage.width(); x++) {

            int pixelX = round(x / xScale);
            int pixelY = round(y / yScale);

            red = auxImage.pixelColor(pixelX, pixelY).red();
            green = auxImage.pixelColor(pixelX, pixelY).green();
            blue = auxImage.pixelColor(pixelX, pixelY).blue();

            aux.setRed(red);
            aux.setGreen(green);
            aux.setBlue(blue);

            m_rimage.setPixelColor(x, y, aux);
        }

    }
    return m_rimage;
}

QImage NativeProcessor::bilinealScale(float xScale, float yScale)
{
    QImage auxImage = m_image.copy(m_rect);

    int newHeight = (float)auxImage.height() * yScale;
    int newWidth = (float)auxImage.width() * xScale;

    new(&m_rimage) QImage(newWidth, newHeight,QImage::Format_RGBA64);
    int red;
    int green;
    int blue;
    QColor aux;

    for (int y = 0; y < m_rimage.height(); y++) {
        for (int x = 0; x < m_rimage.width(); x++) {

            int pixelXA = y / yScale;
            int pixelYA = y / yScale;

            int pixelXB = round(x / xScale);
            int pixelYB = y / yScale;

            int pixelXC = x / xScale;
            int pixelYC = round(y / yScale);

            int pixelXD = round(x / xScale);
            int pixelYD = round(y / yScale);

            int pixelX = (pixelXA + pixelXB + pixelXC + pixelXD)/4;
            int pixelY = (pixelYA + pixelYB + pixelYC + pixelYD)/4;


            red = auxImage.pixelColor(pixelX, pixelY).red();
            green = auxImage.pixelColor(pixelX, pixelY).green();
            blue = auxImage.pixelColor(pixelX, pixelY).blue();

            aux.setRed(red); aux.setGreen(green);
            aux.setBlue(blue);

            m_rimage.setPixelColor(x, y, aux);
        }

    }
    return m_rimage;
}
