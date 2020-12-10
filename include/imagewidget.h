#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QPoint>
#include <QMouseEvent>

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
   void toGrayscale(bool);
   void mousePressEvent(QMouseEvent* eventPress);
   void mouseReleaseEvent(QMouseEvent* eventRelease);
   void mouseMoveEvent(QMouseEvent* eventMove);

   void resizeEvent(QResizeEvent *event);

signals:
   void mouseMoved(QPoint& pos);
   void newImage(QImage& image, QString& format);


};

#endif // IMAGEWIDGET_H
