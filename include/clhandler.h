#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <CL/cl2.hpp>
#include <QImage>
#include <iostream>
#include <tuple>
#include <QDebug>
#include <fstream>
#include <sstream>
#include <filesystem>

enum TransformationFlags
{
    GAUSSIAN  = 1,
    GRAYSCALE = 2,
    BLUR   = 4,
    TEST = 8
};

inline TransformationFlags operator|(TransformationFlags a, TransformationFlags b)
{
    return static_cast<TransformationFlags>(static_cast<int>(a) | static_cast<int>(b));
}

class clHandler
{
private:
    cl::Platform m_default_platform;
    cl::Device m_default_device;
    cl::Context m_context;
    cl::Program m_program;
    cl::Program::Sources m_sources;

    QImage m_qimage;
    //std::vector<std::tuple<int,int,int>> m_image;

    typedef struct Rgba {
        int red;
        int green;
        int blue;
        int a = 0;
    } Rgba;

    Rgba *m_image;

    bool clSetup();
    bool clProcessImage();
    bool configImage();
    bool saveImage();
public:
    clHandler();
    ~clHandler();
    clHandler(QImage image);
    bool clKernelSetup(TransformationFlags transformation);
    bool runKernel();
};

#endif // IMAGEPROCESSING_H
