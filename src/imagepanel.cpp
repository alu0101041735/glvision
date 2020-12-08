#include "imagepanel.h"

imagePanel::imagePanel(QWidget* parent): ui(new Ui::Form)
{
    ui->setupUi(this);
}

void imagePanel::updateMousePos(QPoint *pos)
{
    QString posString = QString("Current pos: %1, %2").arg(pos->x()).arg(pos->y());
    ui->mousePos->setText(posString);
}
