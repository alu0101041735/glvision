#include "mainwindow.h"

#include <iostream>
#include <QDebug>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//
//    std::vector<cl::Platform> platforms;
//    cl::Platform::get(&platforms);
//
//    foreach(auto platform, platforms) {
//        qDebug() << platform.getInfo<CL_PLATFORM_NAME>().data();
//    }
//
//    std::vector<cl::Device> devices;
//    platforms.front().getDevices(CL_DEVICE_TYPE_ALL,  &devices);
//
//        std::cout << "wrf\n";
//    foreach(auto device, devices) {
//
//        qDebug() << device.getInfo<CL_DEVICE_NAME>().data();
//    }
//


    MainWindow w;
    w.show();
    return a.exec();
}
