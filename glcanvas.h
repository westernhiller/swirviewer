#ifndef GLCANVAS_H
#define GLCANVAS_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QKeyEvent>
#include <QMutex>
#include "global.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram);
QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)

class GLCanvas : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GLCanvas(QWidget *parent = nullptr, QImage imgSwir = QImage());
    ~GLCanvas() override;

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
    void setClearColor(const QColor &color);
    inline QImage getImage() { return m_image;}
    void resizeCanvas(int width, int height);
signals:
    void getImage(QImage image);
    void sendMessage(QString);
<<<<<<< HEAD
    void pixelSelect(QPoint);
=======
    void boxSelect(QRect);
>>>>>>> ced1115eb0e78d7e399af6991fd76655586a0538
    void clicked();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *event) override;
<<<<<<< HEAD
=======
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
>>>>>>> ced1115eb0e78d7e399af6991fd76655586a0538

private:
    void makeObject();

    QColor m_clearColor;
    QImage m_image;
    QMutex m_mutex;
    QOpenGLTexture *m_texture;
    QOpenGLShaderProgram *m_program;
    QOpenGLBuffer m_vbo;
    SWIRSETTINGS*   m_pSettings;
    QPoint m_lastPos;

private slots:
    void    onTimer();

public slots:
    void    updateImage(QImage);
    void    exportImage();
};

#endif
