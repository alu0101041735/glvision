#include "mainwindow.h"

#include <QApplication>
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{  
    QSurfaceFormat fmt;
    fmt.setSamples(4);
    QSurfaceFormat::setDefaultFormat(fmt);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    return a.exec();
}
