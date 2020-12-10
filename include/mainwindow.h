#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "imagetab.h"
#include "nativeprocessor.h"
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_File_triggered();
public slots:
    void receieveImage(QImage &image, QString& format);
    void closeTab(int index);

private:
    Ui::MainWindow *ui;
    //Helper helper;
    QMap<uint, imageTab*> tabs;
    void createTab(QImage& image, QString format, QString title);
};
#endif // MAINWINDOW_H
