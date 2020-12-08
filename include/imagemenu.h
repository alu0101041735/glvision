#ifndef IMAGEMENU_H
#define IMAGEMENU_H

#include <QMenu>
#include <QObject>
#include "imagewidget.h"

class imageMenu : public QMenu
{
    Q_OBJECT
public:
    imageMenu(imageWidget* parent = nullptr);
private:
    void buildMenu(imageWidget* parent);
};

#endif // IMAGEMENU_H
