#include "imagewidget.h"
#include <QDebug>
#include <QMenu>
#include <QInputDialog>
#include <QAction>
#include "imagemenu.h"
#include "nativeprocessor.h"
#include <QFileDialog>
#include <QStandardPaths>

imageWidget::imageWidget(QWidget *parent) : QGraphicsView(parent)
{
    processor = nullptr;
    scene = new QGraphicsScene(this);
    this->setScene(scene);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),this, SLOT(ShowContextMenu(const QPoint &)));
    setMouseTracking(true);
}

imageWidget::imageWidget(QImage image, QWidget *parent) :
    QGraphicsView(parent),
    processor(new NativeProcessor(image))
{
    this->image = image;
    scene = new QGraphicsScene(this);
    this->setScene(scene);
    display(image);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    setMouseTracking(true);
}

void imageWidget::resizeEvent(QResizeEvent *event) {
    QGraphicsView::resizeEvent(event);
    fitInView(sceneRect(), Qt::KeepAspectRatio);
}

void imageWidget::display(QImage& image)
{
    this->image = image;
    imagePixItem = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    scene->addItem(imagePixItem);

    show();
}

QImage imageWidget::getImage()
{
    return image;
}

void imageWidget::ShowContextMenu(const QPoint &pos)
{
    QPoint point = mapToGlobal(pos);

    imageMenu menu(this);
    menu.exec(point);
}

void imageWidget::toGrayscale(bool)
{
    if (processor == nullptr) {
       processor = new NativeProcessor(this->image);

    }
    QImage grayImage = processor->getGrayScale();
    QString fileType = tr("test");
    emit newImage(grayImage, fileType);

}

void imageWidget::linTransform()
{
    if (processor == nullptr) {
       processor = new NativeProcessor(this->image);
    }
    bool* ok = new bool(false);
    int numPoints = QInputDialog::getInt(this, tr("How many points"),
                                         tr(""), 0, 0, 20, 1,ok
                                         );
    if (*ok) {
        QList<QString> points;
        for (int i = 0; i < numPoints; ++i)
            points.append(
                        QInputDialog::getText(
                            this,
                            tr("Point"),
                            tr(""),
                            QLineEdit::Normal,
                            tr("Insert the two numbers separated by a comma"),
                            ok
                            )
                        );
        std::vector<std::pair<int, int>> result;
        for (auto pointStr : points) {
           auto pointStrList = pointStr.split(',');
           result.push_back(std::make_pair(
                                pointStrList[0].toInt(),
                                pointStrList[1].toInt()
                                ));
        }

        QImage nImage = processor->processStretch(result);
        QString fileType = tr("test");
        emit newImage(nImage, fileType);
    }
}

void imageWidget::adjustBrightnes()
{
    if (processor == nullptr) {
       processor = new NativeProcessor(this->image);
    }
    bool* ok = new bool(false);
    float multiplier;
    multiplier = QInputDialog::getDouble(this, tr("Brightness"),
                                         tr("New brightness"), 50, 0, 255,
                                         2, ok, Qt::WindowFlags(), 0.01
                                         );
    if (*ok) {
        QImage image = processor->modifyBrightness(multiplier);
        QString Filetype = tr("test");
        emit newImage(image, Filetype);
    }
}

void imageWidget::adjustContrast()
{
    if (processor == nullptr) {
       processor = new NativeProcessor(this->image);
    }
    bool* ok = new bool(false);
    int newContrast;
    newContrast = QInputDialog::getDouble(this, tr("Contrast"),
                                         tr("New contrast"), 75, 0, 128,
                                         2, ok, Qt::WindowFlags(), 0.01
                                         );
    if (*ok) {
        QImage image = processor->modifyContrast(newContrast);
        QString Filetype = tr("test");
        emit newImage(image, Filetype);
    }

}

void imageWidget::equalizeHistogram()
{
    if (processor == nullptr) {
       processor = new NativeProcessor(this->image);
    }

    QImage nImage = processor->equalizeHistogram();
    QString format = tr("test");
    emit newImage(nImage, format);

}

void imageWidget::specifyHistogram()
{
    if (processor == nullptr) {
       processor = new NativeProcessor(this->image);
    }
    QUrl fileUrl;
    QList<QUrl> urls;
    QList<QUrl> file;

    urls.append(QUrl::fromLocalFile(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first()));
    urls.append(QUrl::fromLocalFile(QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).first()));
    urls.append(QUrl::fromLocalFile(QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).first()));

    QFileDialog fileDialog(this, "Open file");
    fileDialog.setSidebarUrls(urls);
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setFilter(QDir::Files | QDir::Dirs | QDir::Drives | QDir::NoDotAndDotDot);
    fileDialog.setNameFilter(QString("*.png *.jpg *.tif"));
    fileDialog.setDirectoryUrl(urls.first());
    fileDialog.exec();
    file = fileDialog.selectedUrls();

    QImage image(file.first().path());
    auto histogram = NativeProcessor(image).getNormalizedCumulativeHistogram();
    image = processor->specifyHistogram(histogram);
    QString format = tr("test");
    emit newImage(image, format);
}

void imageWidget::gammaCorrection()
{
    if (processor == nullptr) {
       processor = new NativeProcessor(this->image);
    }
    bool* ok = new bool(false);
    float multiplier;
    multiplier = QInputDialog::getDouble(this, tr("Gamma"),
                                         tr("Multiplier"), 1, 0, 1000,
                                         2, ok, Qt::WindowFlags(), 0.01
                                         );
    if (*ok) {
        QImage image = processor->gammaCorrection(multiplier);
        QString Filetype = tr("test");
        emit newImage(image, Filetype);
    }

}

void imageWidget::imgCrossSection()
{
    if (processor == nullptr) {
       processor = new NativeProcessor(this->image);
    }

}

void imageWidget::digitalization()
{
    if (processor == nullptr) {
       processor = new NativeProcessor(this->image);
    }

}

void imageWidget::imageDifference()
{
    if (processor == nullptr) {
       processor = new NativeProcessor(this->image);
    }
    QUrl fileUrl;
    QList<QUrl> urls;
    QList<QUrl> file;

    urls.append(QUrl::fromLocalFile(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first()));
    urls.append(QUrl::fromLocalFile(QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).first()));
    urls.append(QUrl::fromLocalFile(QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).first()));

    QFileDialog fileDialog(this, "Open file");
    fileDialog.setSidebarUrls(urls);
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setFilter(QDir::Files | QDir::Dirs | QDir::Drives | QDir::NoDotAndDotDot);
    fileDialog.setNameFilter(QString("*.png *.jpg *.tif"));
    fileDialog.setDirectoryUrl(urls.first());
    fileDialog.exec();
    file = fileDialog.selectedUrls();

    QImage image(file.first().path());
    image = processor->imageDifference(image);
    QString format = tr("test");
    emit newImage(image, format);
}

void imageWidget::mirror()
{
   if  (processor == nullptr)
   {
      processor = new NativeProcessor(this->image);
   }

   bool* ok = new bool(false);
   QStringList items;
   items << tr("vertical") << tr("Horizontal");
   QString option = QInputDialog::getItem(this, tr("Mirror selection"), tr(""), items, 0, 0, ok, Qt::WindowFlags());

   QImage image;
   if (option == items[0])
   {
       image = processor->vMirror();
   } else if (option == items[1])
   {
       image = processor->hMirror();
   }
   QString format = tr("test");
   emit newImage(image, format);
}

void imageWidget::rotate()
{
    if  (processor == nullptr)
    {
      processor = new NativeProcessor(this->image);
    }

    bool* ok = new bool(false);
    short degrees = QInputDialog::getInt(this, tr("Rotation"),
                                         tr("Degrees"), 0, 0, 359,
                                         2, ok, Qt::WindowFlags()
                                         );
    if (!*ok) return;

    bool rotIsRight = (degrees == 90) |
            (degrees == 180) |
            (degrees == 270);

    QImage image;
    if (rotIsRight)
    {
        image = processor->basicRotation(degrees);
    } else
    {
        image = processor->rotateVMP(degrees);
    }

    QString format = tr("test");
    emit newImage(image, format);
}

void imageWidget::rotateBilineal()
{
    if  (processor == nullptr)
    {
      processor = new NativeProcessor(this->image);
    }

    bool* ok = new bool(false);
    short degrees = QInputDialog::getInt(this, tr("Rotation"),
                                         tr("Degrees"), 0, 0, 359,
                                         2, ok, Qt::WindowFlags()
                                         );

    if (!*ok) return;

    bool rotIsRight = (degrees == 90) |
            (degrees == 180) |
            (degrees == 270);

    QImage image;
    if (rotIsRight)
    {
        image = processor->basicRotation(degrees);
    } else
    {
        image = processor->rotateVMP(degrees);
    }
    QString format = tr("test");
    emit newImage(image, format);
}

void imageWidget::rescale()
{
    bool* ok = new bool(false);
    float xScale = QInputDialog::getDouble(this, tr("X axis scaling factor"),
                                         tr("Multiplier"), 1, 0, 1000,
                                         2, ok, Qt::WindowFlags(), 0.01
                                         );
    if (!*ok) return;
    float yScale = QInputDialog::getDouble(this, tr("Y axis scaling factor"),
                                         tr("Multiplier"), 1, 0, 1000,
                                         2, ok, Qt::WindowFlags(), 0.01
                                         );
    if (!*ok) return;

   QImage image = processor->scale(xScale, yScale);
   QString format = tr("test");

   emit newImage(image, format);
}

void imageWidget::rescaleBilineal()
{
    bool* ok = new bool(false);
    float xScale = QInputDialog::getDouble(this, tr("X axis scaling factor"),
                                         tr("Multiplier"), 1, 0, 1000,
                                         2, ok, Qt::WindowFlags(), 0.01
                                         );
    if (!*ok) return;
    float yScale = QInputDialog::getDouble(this, tr("Y axis scaling factor"),
                                         tr("Multiplier"), 1, 0, 1000,
                                         2, ok, Qt::WindowFlags(), 0.01
                                         );
    if (!*ok) return;

   QImage image = processor->bilinealScale(xScale, yScale);
   QString format = tr("test");

   emit newImage(image, format);
}

void imageWidget::transpose()
{
   QImage image = processor->transposed();
   QString format = tr("test");

   emit newImage(image, format);
}

void imageWidget::mousePressEvent(QMouseEvent *eventPress)
{
    if (eventPress->button() == Qt::LeftButton)
    {
        QPoint scenePos = mapToScene(eventPress->pos()).toPoint();
        QPoint imagePos = imagePixItem->mapFromScene(scenePos).toPoint();
        pressEventPos = imagePos;
    }
}

void imageWidget::mouseReleaseEvent(QMouseEvent *eventRelease)
{
    if (eventRelease->button() == Qt::LeftButton)
    {
        QPoint scenePos = mapToScene(eventRelease->pos()).toPoint();
        QPoint imagePos = imagePixItem->mapFromScene(scenePos).toPoint();
        releaseEventPos = imagePos;

        if (!pressEventPos.isNull())
        {
            std::pair<int, int> origin;
            std::pair<int, int> end;
            origin = std::make_pair(
                        pressEventPos.x(),
                        pressEventPos.y()
                        );
            end = std::make_pair(
                        releaseEventPos.x(),
                        releaseEventPos.y()
                        );
            processor->setZone(origin, end);
        }
    }

}

void imageWidget::mouseMoveEvent(QMouseEvent *eventMove)
{
    QPoint mousePos = eventMove->pos();
    QPoint scenePos = mapToScene(mousePos).toPoint();
    QPoint imagePos = imagePixItem->mapFromScene(scenePos).toPoint();
    if (processor == nullptr) {
       processor = new NativeProcessor(this->image);
    }
    int value = processor->grayLevel(imagePos.x(), imagePos.y()).second;
    auto rect = items().first()->sceneBoundingRect().toRect();

    if (rect.contains(imagePos))
    {
        emit mouseMoved(imagePos, value);
    }
}
