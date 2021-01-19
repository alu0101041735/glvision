#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QPoint>
#include <QMouseEvent>
#include "nativeprocessor.h"

class imageWidget : public QGraphicsView
{
    Q_OBJECT
public:
    explicit imageWidget(QWidget *parent = nullptr);
    explicit imageWidget(QImage image, QWidget *parent = nullptr);

    void display(QImage& image);
    QImage getImage(void);

private:
    QGraphicsScene* scene;
    QGraphicsPixmapItem* imagePixItem;
    QImage image;
    NativeProcessor* processor;
    QPoint pressEventPos;
    QPoint releaseEventPos;


public slots:
   void ShowContextMenu(const QPoint &);

   void toGrayscale(bool);
   void linTransform();
   void adjustBrightnes();
   void adjustContrast();
   void equalizeHistogram();
   void specifyHistogram();
   void gammaCorrection();
   void imgCrossSection();
   void digitalization();
   void imageDifference();
   void mirror();
   void rotate();
   void rotateBilineal();
   void rescale();
   void rescaleBilineal();
   void transpose();

   void mousePressEvent(QMouseEvent* eventPress);
   void mouseReleaseEvent(QMouseEvent* eventRelease);
   void mouseMoveEvent(QMouseEvent* eventMove);

   void resizeEvent(QResizeEvent *event);

signals:
   void mouseMoved(QPoint& pos, int value);
   void newImage(QImage& image, QString& format);


};

#endif // IMAGEWIDGET_H
