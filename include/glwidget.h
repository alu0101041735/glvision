#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>

//! [0]
class Helper;

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    GLWidget(Helper *helper, QWidget *parent);

public slots:
    void animate();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Helper *helper;
    int elapsed;
};
//! [0]
#endif // GLWIDGET_H
