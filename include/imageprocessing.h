#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <CL/cl2.hpp>
#include <QImage>
#include <iostream>


class Imageprocessing
{
private:
    cl::Platform m_default_platform;
    cl::Device m_default_device;
    cl::Context m_context;
    cl::Program m_program;
    cl::Program::Sources m_sources;

    QImage m_image;


public:
    Imageprocessing();
    Imageprocessing(QImage image);
    bool clSetup();
    bool clProcessImage();
    bool clKernelSetup();
    bool runKernel();
    bool configImage();
};

#endif // IMAGEPROCESSING_H
