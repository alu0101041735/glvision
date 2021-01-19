#ifndef IMAGETAB_H
#define IMAGETAB_H

#include <QWidget>
#include <QMainWindow>

namespace Ui {
class imageTab;
}

class imageTab : public QWidget
{
    Q_OBJECT

public:
    explicit imageTab(QWidget *parent = nullptr);
    explicit imageTab(QImage& image, QString& format, QWidget *parent = nullptr);
    ~imageTab();

    void connectImageReturn();
    void connectImageReturn(QMainWindow* receiever);

public slots:
   void updateHistogram();

private:
    Ui::imageTab *ui;
    bool histogramMode;
    QImage image_t;

};

enum HistogramMode {normal, cummulative};
#endif // IMAGETAB_H
