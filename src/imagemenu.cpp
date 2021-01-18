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
    QAction* toGray = new QAction(tr("To grayscale"), this->parent());
    QAction* linearTransformation = new QAction(tr("Linear transformation"), this->parent());
    QAction* adjustBrightness = new QAction(tr("Adjust Brightness"), this->parent());
    QAction* adjustContrast= new QAction(tr("Adjust Contrast"), this->parent());
    QAction* equalizeHistogram = new QAction(tr("Equalize histogram"), this->parent());
    QAction* specifyHistogram = new QAction(tr("Specify histogram"), this->parent());
    QAction* gammaCorrection = new QAction(tr("Gamma correction"), this->parent());
    QAction* imageDifference = new QAction(tr("Image difference"), this->parent());
    QAction* rotation = new QAction(tr("Rotate"), this->parent());
    QAction* rotationBilineal = new QAction(tr("Rotate (Bilineal)"), this->parent());
    QAction* mirroring = new QAction(tr("Mirror"), this->parent());
    QAction* rescaling = new QAction(tr("Rescale"), this->parent());
    QAction* rescalingBilineal = new QAction(tr("Rescale (Bilineal)"), this->parent());
    QAction* transpositon = new QAction(tr("Transpose"), this->parent());

    //Connections
    connect(toGray, &QAction::triggered, parent, &imageWidget::toGrayscale);
    connect(linearTransformation, &QAction::triggered, parent, &imageWidget::linTransform);
    connect(adjustBrightness, &QAction::triggered, parent, &imageWidget::adjustBrightnes);
    connect(adjustContrast, &QAction::triggered, parent, &imageWidget::adjustContrast);
    connect(equalizeHistogram, &QAction::triggered, parent, &imageWidget::equalizeHistogram);
    connect(specifyHistogram, &QAction::triggered, parent, &imageWidget::specifyHistogram);
    connect(gammaCorrection, &QAction::triggered, parent, &imageWidget::gammaCorrection);
    connect(imageDifference, &QAction::triggered, parent, &imageWidget::imageDifference);
    connect(rotation, &QAction::triggered, parent, &imageWidget::rotate);
    connect(rotationBilineal, &QAction::triggered, parent, &imageWidget::rotateBilineal);
    connect(mirroring, &QAction::triggered, parent, &imageWidget::mirror);
    connect(rescaling, &QAction::triggered, parent, &imageWidget::rescale);
    connect(rescalingBilineal, &QAction::triggered, parent, &imageWidget::rescaleBilineal);
    connect(transpositon, &QAction::triggered, parent, &imageWidget::transpose);


    addAction(toGray);
    addAction(linearTransformation);
    addAction(adjustBrightness);
    addAction(adjustContrast);
    addAction(equalizeHistogram);
    addAction(specifyHistogram);
    addAction(gammaCorrection);
    addAction(imageDifference);
    addAction(rotation);
    addAction(rotationBilineal);
    addAction(mirroring);
    addAction(rescaling);
    addAction(rescalingBilineal);
    addAction(transpositon);


}
