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

private:
    Ui::imageTab *ui;

};

#endif // IMAGETAB_H
