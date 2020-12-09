#include "imagepanel.h"
#include "nativeprocessor.h"
#include <QtCharts>

imagePanel::imagePanel(QWidget* parent): QWidget(parent), ui(new Ui::Form)
{
    ui->setupUi(this);
}

void imagePanel::setHistrogram(QImage& image)
{
    std::vector<uint32_t> histogram = NativeProcessor(image).getHistogram();
    QBarSet* barSet = new QBarSet("histogram");
    for (auto value : histogram) {
        *barSet << value;
        qDebug() << value;
    }

    QPen pen(Qt::black, 5);
    barSet->setPen(pen);
    QBarSeries* series = new QBarSeries();
    series->append(barSet);
    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Histogram");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    //QChartView* view = new QChartView(chart, this->ui->histogramChart);
    chart->resize(this->ui->graphicsView->size());
    this->ui->graphicsView->setChart(chart);
    this->ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    //this->ui->graphicsView->fitInView(chart->boundingRect(), Qt::KeepAspectRatio);
}

void imagePanel::updateMousePos(QPoint &pos)
{
    QString posString = QString("Current pos: %1, %2").arg(pos.x()).arg(pos.y());
    ui->mousePos->setText(posString);
}
