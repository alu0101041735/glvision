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
    this->ui->lumRangeMin->setText(
                QString::number(info.getRange().first)
                );
    this->ui->lumRangeMax->setText(
                QString::number(info.getRange().second)
                );

}

void imagePanel::setHistrogram(QImage& image, bool cummulative)
{
    std::vector<uint32_t> histogram;
    NativeProcessor processor = NativeProcessor(image);
    if (!cummulative)
        histogram = processor.getHistogram();
    else
        histogram = processor.getCumulativeHistogram();
    QBarSet* barSet = new QBarSet("histogram");
    for (auto value : histogram) {
        *barSet << value;
    }

    QPen pen(Qt::black, 5);
    barSet->setPen(pen);
    QBarSeries* series = new QBarSeries();
    series->append(barSet);
    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->resize(this->ui->graphicsView->size());
    this->ui->graphicsView->setChart(chart);
    this->ui->graphicsView->setRenderHint(QPainter::Antialiasing);
}

void imagePanel::updateMousePos(QPoint &pos, int value)
{
    ui->posX->setText(QString::number(pos.x()));
    ui->posY->setText(QString::number(pos.y()));
    ui->luminosity->setText(QString::number(value));
}

void imagePanel::on_toggleHistogram_clicked()
{
    emit toggleHistrogram();
}
