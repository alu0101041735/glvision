#include "imagewidget.h"
#include <QDebug>
#include <QMenu>
#include <QAction>
#include "imagemenu.h"

imageWidget::imageWidget(QWidget *parent) : QGraphicsView(parent)
{
    scene = new QGraphicsScene(this);
    this->setScene(scene);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),this, SLOT(ShowContextMenu(const QPoint &)));
    setMouseTracking(true);
}

imageWidget::imageWidget(QImage image, QWidget *parent) : QGraphicsView(parent)
{
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
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    scene->addItem(item);

    show();
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
    qDebug() << eventMove->pos();
    QPoint mousePos = eventMove->pos();
    auto item = this->items().first();
    auto itemPos = item->pos();
    auto rect = item->sceneBoundingRect().toRect();
    qreal px = itemPos.rx();
    qreal py = itemPos.ry();
    rect = rect.adjusted(px, py, px, py);
    if (rect.contains(mousePos))
    {
        mousePos += itemPos.toPoint();
        qDebug() << mousePos;
        qDebug() << this->pos();
        emit mouseMoved(mousePos);
    }

}
