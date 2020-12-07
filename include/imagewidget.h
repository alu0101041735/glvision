#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QPoint>

class imageWidget : public QGraphicsView
{
    Q_OBJECT
public:
    explicit imageWidget(QWidget *parent = nullptr);
    explicit imageWidget(QImage image, QWidget *parent = nullptr);

    void display(QImage& image);

private:
    QGraphicsScene* scene;
    QGraphicsPixmapItem* imagePixItem;
    QImage image;

public slots:
   void ShowContextMenu(const QPoint &);

signals:


};

#endif // IMAGEWIDGET_H
