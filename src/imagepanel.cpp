#include "imagepanel.h"
#include "nativeprocessor.h"
#include <QtCharts>

imagePanel::imagePanel(QWidget* parent): QWidget(parent), ui(new Ui::Form)
{
    ui->setupUi(this);
}

void imagePanel::updateInfo(imageInfo &info)
{
    this->info = info;
    this->ui->fileType->setText(info.getFormat());
    this->ui->sizeH->setText(QString::number(info.getDimensions().first));
    this->ui->sizeW->setText(QString::number(info.getDimensions().second));
    this->ui->brightness->setText(
                QString::number(info.getBrightness())
                );
    this->ui->contrast->setText(
                QString::number(info.getContrast())
                );
    this->ui->entropy->setText(
                QString::number(info.getEntropy())
                );

}

void imagePanel::setHistrogram(QImage& image)
{
    std::vector<uint32_t> histogram = NativeProcessor(image).getHistogram();
    QBarSet* barSet = new QBarSet("histogram");
    for (auto value : histogram) {
        *barSet << value;
        //qDebug() << value;
    }

    QPen pen(Qt::black, 5);
    barSet->setPen(pen);
    QBarSeries* series = new QBarSeries();
    series->append(barSet);
    QChart* chart = new QChart();
    chart->addSeries(series);
    //chart->setTitle("Histogram");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    //QChartView* view = new QChartView(chart, this->ui->histogramChart);
    chart->resize(this->ui->graphicsView->size());
    this->ui->graphicsView->setChart(chart);
    this->ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    //this->ui->graphicsView->fitInView(chart->boundingRect(), Qt::KeepAspectRatio);
}

void imagePanel::updateMousePos(QPoint &pos, int value)
{
    QString posString = QString("Current pos: %1, %2 value: %3").arg(pos.x()).arg(pos.y()).arg(value);
    ui->mousePos->setText(posString);
}
