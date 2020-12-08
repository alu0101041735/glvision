#include "imagemenu.h"
#include "imagewidget.h"

imageMenu::imageMenu(imageWidget* parent)
{
    buildMenu(parent);
}

void imageMenu::buildMenu(imageWidget* parent)
{
    this->addSection("Transformations");
    //Actions
    QAction* toGray = new QAction(tr("to grayscale"), this->parent());

    //Connections
    connect(toGray, &QAction::triggered, parent, &imageWidget::toGrayscale);
    this->addAction(toGray);

}
