#include "imagetab.h"
#include "ui_imagetab.h"
#include "mainwindow.h"
#include "imageinfo.h"

imageTab::imageTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::imageTab)
{
    ui->setupUi(this);
}

imageTab::imageTab(QImage &image, QString& format, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::imageTab),
    histogramMode(normal),
    image_t(image)
{
    NativeProcessor imagePr(image);
    pair dimensions(imagePr.getHeight(), imagePr.getWidth());
    imageInfo info(format,
                   dimensions,
                   imagePr.valueRange(),
                   imagePr.brightness(),
                   imagePr.contrast(),
                   imagePr.getEntropy()
                );
    ui->setupUi(this);
    ui->image->display(image_t);
    //ui->panel->setHistrogram(image);
    updateHistogram();
    ui->panel->updateInfo(info);
    connect(ui->image, &imageWidget::mouseMoved, ui->panel, &imagePanel::updateMousePos);
    connect(ui->panel, &imagePanel::toggleHistrogram, this, &imageTab::updateHistogram);
}

imageTab::~imageTab()
{
    delete ui;
}

void imageTab::connectImageReturn(QMainWindow *receiever)
{
    connect(this->ui->image, SIGNAL(newImage(QImage&, QString&)),
            receiever, SLOT(receieveImage(QImage&, QString&)));

}

void imageTab::updateHistogram()
{
    if (histogramMode == normal)
    {
        ui->panel->setHistrogram(image_t, normal);
        histogramMode = cummulative;
    }
    else if (histogramMode == cummulative)
    {
        ui->panel->setHistrogram(image_t, cummulative);
        histogramMode = normal;
    }
}
