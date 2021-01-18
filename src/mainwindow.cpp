#include "mainwindow.h"

#include "imagewidget.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QGraphicsPixmapItem>
#include <iostream>
#include <exception>
#include <QDebug>
#include "imagetab.h"
#include <QtCharts>
#include "imageinfo.h"



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

    QImage original = np.getOriginalImage();
    QImage result = np.modifyContrast(100);
    original.save("../glvision/images/original.png");
    result.save("../glvision/images/result_bcontrast.png");

    result = np.modifyBrightness(1.2);
    result.save("../glvision/images/result_brightness.png");

     result = np.gammaCorrection(0.5);
     result.save("../glvision/images/gammacorrection.png");

    np.getGrayScale().save("../glvision/images/grayscale.png");
    QImage test2("../glvision/images/kuvshi.jpg");
    NativeProcessor np2(test2);
    std::vector<double> temp_histogram = np2.getNormalizedCumulativeHistogram();

    np.specifyHistogram(temp_histogram).save("../glvision/images/specification_test.png");
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->tabWidget->setTabsClosable(true);
    ui->tabWidget->setTabBarAutoHide(true);
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);
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
    fileDialog.setNameFilter(QString("*.png *.jpg *.tif"));
    fileDialog.setDirectoryUrl(urls.first());
    fileDialog.exec();
    files = fileDialog.selectedUrls();

    if (files.size() == 0)
        return;

    for (auto fileUrl : files) {
        images.append(QImage(fileUrl.path()));
    }

    try {
        QFileInfo info(files.first().path());
        createTab(images.first(), info.completeSuffix(), files.first().fileName());
    }  catch (std::exception& e) {
    }
}

void MainWindow::receieveImage(QImage &image, QString& format)
{
    createTab(image, format, tr(""));
}

void MainWindow::closeTab(int index)
{
    this->ui->tabWidget->removeTab(index);
}

void MainWindow::createTab(QImage &image, QString format, QString title)
{
    imageTab* newTab = new imageTab(image, format, ui->tabWidget);
    ushort currentTab = ui->tabWidget->currentIndex();
    QString newTitle;
    if (title == "")
    {
        title = ui->tabWidget->tabText(currentTab);
        title = title.split(' ').first();
        tabnameCount[title] += 1;
        newTitle = QString("%1 (%2)").arg(title).arg(tabnameCount[title]);

    } else
    {
        newTitle = title;
    }
    ui->tabWidget->insertTab( ++currentTab, newTab, newTitle);
    newTab->connectImageReturn(this);
}




void MainWindow::on_actionSave_image_triggered()
{
    QUrl fileUrl = QFileDialog::getSaveFileUrl();
    ushort currentTabIndex = ui->tabWidget->currentIndex();
    QWidget* currentTab = ui->tabWidget->widget(currentTabIndex);
    imageWidget* imWidget = currentTab->findChild<imageWidget *>("image");
    imWidget->getImage().save(fileUrl.path());

}
