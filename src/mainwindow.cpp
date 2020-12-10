#include "mainwindow.h"

#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QGraphicsPixmapItem>
#include <iostream>
#include <exception>
#include <QDebug>
#include "imagetab.h"
#include <QtCharts>



void test_all(QImage image)
{
    NativeProcessor np(image);

    std::cout << "Image width: " << np.getWidth();
    std::cout << "\nImage height: " << np.getHeight();
    std::vector<uint32_t> histogram = np.getHistogram();

    std::cout << "\n\nHistogram: ";
    for (int i = 0; i < histogram.size(); i++) {
        std::cout << " " << histogram[i];
    }
    std::cout << "\n";

    histogram = np.getCumulativeHistogram();
    std::cout << "Cumulative Histogram: ";
    for (int i = 0; i < histogram.size(); i++) {
        std::cout << " " << histogram[i];
    }

    std::cout << "\n\nValue Range: " << np.valueRange().first << " " << np.valueRange().second;
    std::cout << "\n\nBrightness: " << np.brightness();
    std::cout << "\n\nContrast: " << np.contrast();
    std::cout << "\n\nEntropy: " << np.getEntropy();

    QImage original = np.getGrayScale();
    QImage result = np.modifyContrast(100);
    original.save("../glvision/images/original.png");
    result.save("../glvision/images/result_brightness.png");

    result = np.modifyBrightness(1.2);
    result.save("../glvision/images/result_contrast.png");

     result = np.gammaCorrection(0.5);
     result.save("../glvision/images/gammacorrection.png");
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    //delete ui;
}


void MainWindow::on_actionOpen_File_triggered()
{
    QUrl fileUrl;
    QList<QUrl> urls;
    QList<QUrl> files;
    QList <QImage> images;

    urls.append(QUrl::fromLocalFile(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first()));
    urls.append(QUrl::fromLocalFile(QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).first()));
    urls.append(QUrl::fromLocalFile(QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).first()));
    QFileDialog fileDialog(this, "Open file");
    fileDialog.setSidebarUrls(urls);
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setFilter(QDir::Files | QDir::Dirs | QDir::Drives | QDir::NoDotAndDotDot);
    fileDialog.setNameFilter(QString("*.png *.jpg"));
    fileDialog.setDirectoryUrl(urls.first());
    fileDialog.exec();
    files = fileDialog.selectedUrls();


    for (auto fileUrl : files) {
        images.append(QImage(fileUrl.path()));
    }

    try {
   //     this->ui->image->display(images.first());
    }  catch (std::exception& e) {
        qDebug() << "test";
    }

    fileUrl = fileUrl.adjusted(QUrl::StripTrailingSlash);
    createTab(images.first(), fileUrl.fileName());
    /*
    imageTab* newTab = new imageTab(images.first(), ui->tabWidget);
    ui->tabWidget->addTab( newTab, urls.first().toString());
    newTab->connectImageReturn(this);
    auto index = ui->tabWidget->currentIndex();
    */

    //this->ui->tabWidget->insertTab(this);
    //tabs.append()
    //this->ui->tabWidget->currentIndex();
    //tabs.append(new imageTab(this->ui->tab_2));

    //Histogrem branch
    


    /*
    clHandler image(images[0]);
    image.clKernelSetup(GRAYSCALE);
    image.runKernel();

    QImage result = image.getImage();

    */

    //NativeProcessor np(images[0]);

}

void MainWindow::receieveImage(QImage &image)
{
   createTab(image, tr("prueba"));
}

void MainWindow::createTab(QImage &image, QString title)
{
    imageTab* newTab = new imageTab(image, ui->tabWidget);
    ui->tabWidget->addTab( newTab, title);
    newTab->connectImageReturn(this);
    //auto index = ui->tabWidget->currentIndex();

}



