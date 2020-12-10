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
    ui(new Ui::imageTab)
{
    NativeProcessor imagePr(image);
    pair dimensions(imagePr.getHeight(), imagePr.getWidth());
    imageInfo info(format,
                   dimensions,
                   pair(0, 0),
                   imagePr.brightness(),
                   imagePr.contrast(),
                   imagePr.getEntropy()
                );
    ui->setupUi(this);
    ui->image->display(image);
    ui->panel->setHistrogram(image);
    ui->panel->updateInfo(info);
    connect(ui->image, &imageWidget::mouseMoved, ui->panel, &imagePanel::updateMousePos);
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
