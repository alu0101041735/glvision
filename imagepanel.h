#ifndef IMAGEPANEL_H
#define IMAGEPANEL_H

#include <QObject>
#include <QWidget>
#include "ui_imagePanel.h"
#include <QGraphicsScene>

class imagePanel : public QWidget
{
    Q_OBJECT
public:
    imagePanel(QWidget* parent = nullptr);
private:
    Ui::Form ui;
    QGraphicsScene scene;

public:
    void setHistrogram(QImage);
};

#endif // IMAGEPANEL_H
