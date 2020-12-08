#include "imagetab.h"
#include "ui_imagetab.h"

imageTab::imageTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::imageTab)
{
    ui->setupUi(this);
}

imageTab::imageTab(QImage &image, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::imageTab)
{
    ui->setupUi(this);
    ui->image->display(image);
    connect(ui->image, &imageWidget::mouseMoved, ui->panel, &imagePanel::updateMousePos);
}

imageTab::~imageTab()
{
    delete ui;
}
