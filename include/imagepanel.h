#ifndef IMAGEPANEL_H
#define IMAGEPANEL_H

#include <QObject>
#include <QWidget>
#include <QGraphicsScene>
#include <QPoint>

#include "ui_imagePanelForm.h"
#include "imageinfo.h"
class imagePanel : public QWidget
{
    Q_OBJECT
public:
    imagePanel(QWidget* parent = nullptr);
private:
    Ui::Form* ui;
    QGraphicsScene scene;
    imageInfo info;

public:
    void updateInfo(imageInfo& info);
    void setHistrogram(QImage& image);

public slots:
    void updateMousePos(QPoint& pos, int value);
};

#endif // IMAGEPANEL_H
