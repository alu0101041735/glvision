#ifndef IMAGEPANEL_H
#define IMAGEPANEL_H

#include <QObject>
#include <QWidget>
#include "ui_imagePanelForm.h"
#include <QGraphicsScene>
#include <QPoint>

class imagePanel : public QWidget
{
    Q_OBJECT
public:
    imagePanel(QWidget* parent = nullptr);
private:
    Ui::Form* ui;
    QGraphicsScene scene;

public:
    void setHistrogram(QImage);

public slots:
    void updateMousePos(QPoint* pos);
};

#endif // IMAGEPANEL_H
