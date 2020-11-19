#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QElapsedTimer>
#include <QOpenGLTexture>
#include <QDebug>
#include <QOffscreenSurface>
#include <QOpenGLFramebufferObject>
#include <iostream>

class Glwidget : public QOpenGLWidget
{
public:
    Glwidget();

    bool createContext();
    bool createSurface();
    bool createProgram();
    bool createTexture();
    bool bindVertIndData();
    bool setProgramFlags(
                        const QString& textureVar,
                        const QString& vertexPosVar,
                        const QString& textureCoordVar);

    QImage processImage(
                        const QString& textureVar,
                        const QString& vertexPosVar,
                        const QString& textureCoordVar);


private:
    QString m_vertexShader;
    QString m_fragmentShader;
    QOpenGLContext m_context;
    QOffscreenSurface m_surface;
    QOpenGLShaderProgram m_program;
    QImage m_image;

    struct VertexData
    {
        QVector2D position;
        QVector2D texCoord;
    };

};

#endif // GLWIDGET_H
