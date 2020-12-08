#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QStandardPaths>
#include "glimage.h"
#include <QtCharts>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
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
    //urls = fileDialog.getOpenFileUrls();
    //fileUrl = fileDialog.getOpenFileUrl(this, "Open image");
    fileDialog.exec();
    files = fileDialog.selectedUrls();

    for (auto fileUrl : files) {
        images.append(QImage(fileUrl.path()));
    }

    QVector<unsigned int> histogram = GlVision::GLImage(images.first()).histogram();
    QBarSet* barSet = new QBarSet("histogram");
    for (auto value : histogram) {
        *barSet << value;
    }
    QBarSeries* series = new QBarSeries();
    series->append(barSet);
    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Histogram");
    QChartView* view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);
    this->setCentralWidget(view);
    /*
    for (auto &url: urls)
    {
    }
    for (auto &url: urls)
    {
    }
    */
}
