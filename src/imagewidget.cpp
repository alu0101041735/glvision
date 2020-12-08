#include "imagewidget.h"
#include <QDebug>
#include <QMenu>
#include <QAction>
#include "imagemenu.h"

imageWidget::imageWidget(QWidget *parent) : QGraphicsView(parent)
{
    scene = new QGraphicsScene;
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),this, SLOT(ShowContextMenu(const QPoint &)));
    setMouseTracking(true);
}

imageWidget::imageWidget(QImage image, QWidget *parent) : QGraphicsView(parent)
{
    scene = new QGraphicsScene;
    display(image);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
}

void imageWidget::display(QImage& image)
{
    this->image = image;
    imagePixItem = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    scene->addItem(imagePixItem);

    this->setScene(scene);
    this->show();
    this->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

void imageWidget::ShowContextMenu(const QPoint &pos)
{
    QPoint point = mapToGlobal(pos);

    imageMenu menu(this);
    menu.exec(point);
    /*
    QMenu* menu = new QMenu(this);
    menu->addAction("First Action");
    menu->exec(point);
    qDebug() << "Showing menu";
    */
}

void imageWidget::toGrayscale(bool)
{
    qDebug() << "to grayscale";
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
    //qDebug() << eventMove->pos();
    QPoint* mousePos = new QPoint(eventMove->pos());
    emit mouseMoved(mousePos);
}
