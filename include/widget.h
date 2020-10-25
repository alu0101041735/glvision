#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

//! [0]
class Helper;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(Helper *helper, QWidget *parent);

public slots:
    void animate();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Helper *helper;
    int elapsed;
};
//! [0]

#endif // WIDGET_H
