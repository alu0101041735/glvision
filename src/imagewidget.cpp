#include "imagewidget.h"
#include <QDebug>
#include <QMenu>
#include <QInputDialog>
#include <QAction>
#include "imagemenu.h"
#include "nativeprocessor.h"

imageWidget::imageWidget(QWidget *parent) : QGraphicsView(parent)
{
    processor = nullptr;
    scene = new QGraphicsScene(this);
    this->setScene(scene);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),this, SLOT(ShowContextMenu(const QPoint &)));
    setMouseTracking(true);
}

imageWidget::imageWidget(QImage image, QWidget *parent) :
    QGraphicsView(parent),
    processor(new NativeProcessor(image))
{
    this->image = image;
    scene = new QGraphicsScene(this);
    this->setScene(scene);
    display(image);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    setMouseTracking(true);
}

void imageWidget::resizeEvent(QResizeEvent *event) {
    QGraphicsView::resizeEvent(event);
    fitInView(sceneRect(), Qt::KeepAspectRatio);
}

void imageWidget::display(QImage& image)
{
    this->image = image;
    imagePixItem = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    scene->addItem(imagePixItem);

    show();
}

void imageWidget::ShowContextMenu(const QPoint &pos)
{
    QPoint point = mapToGlobal(pos);

    imageMenu menu(this);
    menu.exec(point);
}

void imageWidget::toGrayscale(bool)
{
    qDebug() << "to grayscale";
    QImage grayImage = NativeProcessor(this->image).processImage(GRAYSCALE);
    grayImage.save("gray.png");
    QString fileType = tr("test");
    emit newImage(grayImage, fileType);

}

void imageWidget::linTransform()
{

}

void imageWidget::adjustBrightnes()
{
    bool* ok = new bool(false);
    float multiplier;
    multiplier = QInputDialog::getDouble(this, tr("Brightness"),
                                         tr("Multiplier"), 1, 0, 1000,
                                         2, ok, Qt::WindowFlags(), 0.01
                                         );
    if (*ok) {
        QImage image = NativeProcessor(this->image).modifyBrightness(multiplier);
        QString Filetype = tr("test");
        emit newImage(image, Filetype);
    }
}

void imageWidget::adjustContrast()
{
    bool* ok = new bool(false);
    int newContrast;
    newContrast = QInputDialog::getInt(this, tr("Contrast"),
                                         tr("New contrast"), 0, -255, 255,
                                         1, ok, Qt::WindowFlags()
                                         );
    if (*ok) {
        QImage image = NativeProcessor(this->image).modifyContrast(newContrast);
        QString Filetype = tr("test");
        emit newImage(image, Filetype);
    }

}

void imageWidget::equalizeHistogram()
{

}

void imageWidget::specifyHistogram()
{

}

void imageWidget::gammaCorrection()
{
    bool* ok = new bool(false);
    float multiplier;
    multiplier = QInputDialog::getDouble(this, tr("Gamma"),
                                         tr("Multiplier"), 1, 0, 1000,
                                         2, ok, Qt::WindowFlags(), 0.01
                                         );
    if (*ok) {
        QImage image = NativeProcessor(this->image).gammaCorrection(multiplier);
        QString Filetype = tr("test");
        emit newImage(image, Filetype);
    }

}

void imageWidget::imgCrossSection()
{

}

void imageWidget::digitalization()
{

}

void imageWidget::imageDifference()
{

}

void imageWidget::mousePressEvent(QMouseEvent *eventPress)
{
    if (eventPress->button() == Qt::LeftButton)
    {
        qDebug() << tr("Left mouse press event");
        qDebug() << eventPress->pos();
    }
}

void imageWidget::mouseReleaseEvent(QMouseEvent *eventRelease)
{

}

void imageWidget::mouseMoveEvent(QMouseEvent *eventMove)
{
    QPoint mousePos = eventMove->pos();
    QPoint scenePos = mapToScene(mousePos).toPoint();
    QPoint imagePos = imagePixItem->mapFromScene(scenePos).toPoint();
    //QPoint imagePos = scene->

    auto rect = items().first()->sceneBoundingRect().toRect();
    if (rect.contains(imagePos))
    {
        emit mouseMoved(imagePos);
    }
    qDebug() << mousePos << scenePos;

}
