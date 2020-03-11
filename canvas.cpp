#include "canvas.h"
#include <QPainter>

Canvas::Canvas(QWidget *parent)
    : QLabel(parent)
{
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.drawPixmap(rect(), m_img);
}

void Canvas::updateImage(QImage image)
{
    m_img = QPixmap::fromImage(image);
    update();
}


