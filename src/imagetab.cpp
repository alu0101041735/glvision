#include "imagetab.h"
#include "ui_imagetab.h"

imageTab::imageTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::imageTab)
{
    ui->setupUi(this);
}

imageTab::~imageTab()
{
    delete ui;
}
