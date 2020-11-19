#include "glwidget.h"

Glwidget::Glwidget()
{

    if (!m_image.load("/home/daniel/Documents/Prácticas/VPC/glvision/images/mountain.jpg")) {
        qDebug() << "Failed loading image";

    }
    m_vertexShader =
            "attribute vec4 aPosition;\n"
            "attribute vec2 aTexCoord;\n"
            "varying vec2 vTexCoord;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = aPosition;\n"
            "   vTexCoord = aTexCoord;\n"
            "}";

    m_fragmentShader =
            "uniform sampler2D texture;\n"
            "varying vec2 vTexCoord;\n"
            "void main()\n"
            "{\n"
            "   gl_FragColor = texture2D(texture, vTexCoord);\n"
            "}";

    QImage image = processImage(
                                "texture",
                                "aPosition",
                                "aTexCoord");
}


bool Glwidget::createContext()
{

    if(!m_context.create())
    {
        qDebug() << "Can't create GL context.";
        return false;
    }

    return true;
}


bool Glwidget::createSurface()
{
    m_surface.setFormat(m_context.format());
    m_surface.create();
    if(!m_surface.isValid())
    {
        qDebug() << "Surface not valid.";
        return false;
    }
    return true;
}


bool Glwidget::createProgram()
{
    QOpenGLShaderProgram program(&m_context);
    if (!program.addShaderFromSourceCode(QOpenGLShader::Vertex, m_vertexShader))
    {
        qDebug() << "Can't add vertex shader.";
        return false;
    }
    if (!program.addShaderFromSourceCode(QOpenGLShader::Fragment, m_fragmentShader))
    {
        qDebug() << "Can't add fragment shader.";
        return false;
    }
    if (!program.link())
    {
        qDebug() << "Can't link program.";
        return false;
    }
    if (!program.bind())
    {
        qDebug() << "Can't bind program.";
        return false;
    }

    return true;
}


bool Glwidget::createTexture()
{
    QOpenGLTexture texture(QOpenGLTexture::Target2D);
    texture.setData(m_image);

    texture.bind();
    if(!texture.isBound())
    {
        qDebug() << "Texture not bound.";
        return false;
    }
    return true;
}

bool Glwidget::bindVertIndData()
{

    VertexData vertices[] =
    {
        {{ -1.0f, +1.0f }, { 0.0f, 1.0f }}, // top-left
        {{ +1.0f, +1.0f }, { 1.0f, 1.0f }}, // top-right
        {{ -1.0f, -1.0f }, { 0.0f, 0.0f }}, // bottom-left
        {{ +1.0f, -1.0f }, { 1.0f, 0.0f }}  // bottom-right
    };

    GLuint indices[] =
    {
        0, 1, 2, 3
    };

    QOpenGLBuffer vertexBuf(QOpenGLBuffer::VertexBuffer);
    QOpenGLBuffer indexBuf(QOpenGLBuffer::IndexBuffer);

    if(!vertexBuf.create())
    {
        qDebug() << "Can't create vertex buffer.";
        return {};
    }

    if(!indexBuf.create())
    {
        qDebug() << "Can't create index buffer.";
        return {};
    }

    if(!vertexBuf.bind())
    {
        qDebug() << "Can't bind vertex buffer.";
        return {};
    }
    vertexBuf.allocate(vertices, 4 * sizeof(VertexData));

    if(!indexBuf.bind())
    {
        qDebug() << "Can't bind index buffer.";
        return {};
    }
    indexBuf.allocate(indices, 4 * sizeof(GLuint));

}


bool Glwidget::setProgramFlags(
                        const QString& textureVar,
                        const QString& vertexPosVar,
                        const QString& textureCoordVar)
{
    int offset = 0;
    m_program.enableAttributeArray(vertexPosVar.toLatin1().data());
    m_program.setAttributeBuffer(vertexPosVar.toLatin1().data(), GL_FLOAT, offset, 2, sizeof(VertexData));
    offset += sizeof(QVector2D);
    m_program.enableAttributeArray(textureCoordVar.toLatin1().data());
    m_program.setAttributeBuffer(textureCoordVar.toLatin1().data(), GL_FLOAT, offset, 2, sizeof(VertexData));
    m_program.setUniformValue(textureVar.toLatin1().data(), 0);

    return true;
}


QImage Glwidget::processImage(
                                        const QString& textureVar,
                                        const QString& vertexPosVar,
                                        const QString& textureCoordVar)
{
    if (!createContext()) {
        qDebug() << "Failed creating context";
    }

    if (!createSurface()) {
        qDebug() << "Failed creating surface";
    }


    if(!m_context.makeCurrent(&m_surface))
    {
     qDebug() << "Can't make context current.";
    }
    QOpenGLFramebufferObject fbo(m_image.size());
    m_context.functions()->glViewport(0, 0, m_image.width(), m_image.height());

    if (!createProgram()) {
     qDebug() << "Failed creating shader program.";
    }
    if (!createTexture()) {
     qDebug() << "Can't bind texture.";
    }

    if (!bindVertIndData()) {
        qDebug() << "Failed binding vertex and index data";
    }

    if (!setProgramFlags(textureVar, vertexPosVar, textureCoordVar)) {
        qDebug() << "Failed to set flags";
    }

    m_context.functions()->glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, Q_NULLPTR);
    return fbo.toImage(false);

}
