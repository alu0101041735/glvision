#include "mainwindow.h"

#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QGraphicsPixmapItem>
#include <iostream>
#include <exception>
#include <QDebug>
#include "imagetab.h"

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
        this->ui->image->display(images.first());
    }  catch (std::exception& e) {
        qDebug() << "test";
    }
    /*
    clHandler image(images[0]);
    image.clKernelSetup(GRAYSCALE);
    image.runKernel();

    QImage result = image.getImage();

    NativeProcessor np(images[0 ]);
    QImage test = np.processImage(0);
    np.saveImage();

    */

    this->ui->tabWidget->currentIndex();
    tabs.append(new imageTab(this->ui->tab_2));
}
