#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H


#include <QImage>
#include <iostream>
#include <tuple>
#include <QDebug>
#include <fstream>
#include <sstream>
#include <filesystem>



typedef  struct Rgba {
    cl_int red;
    cl_int green;
    cl_int blue;
    cl_int a = 255;
} __attribute__((aligned(128)))Rgba;

class clHandler
{
private:
    cl::Platform m_default_platform;
    cl::Device m_default_device;
    cl::Context m_context;
    cl::Program m_program;
    cl::Program::Sources m_sources;

    QImage m_qimage;
    QImage m_qimageresult;

    Rgba *m_image;
    unsigned long int m_size;
    int m_width;
    int m_height;

    std::string m_kernelname;

    TransformationFlags m_flag;

    bool clSetup();
    bool clProcessImage();
    bool configImage();
    bool saveImage(Rgba *image);
public:
    clHandler();
    ~clHandler();
    clHandler(QImage image);
    bool clKernelSetup(TransformationFlags transformation);
    bool runKernel();
    QImage getImage();
};

#endif // IMAGEPROCESSING_H
