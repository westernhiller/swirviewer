#include "imagebutton.h"
#include <QPainter>
#include <stdio.h>

ImageButton::ImageButton(QString image1, QString image2, bool bRestoreOnRelease, QWidget *parent)
    : QLabel(parent)
    , m_bHighlighted(false)
    , m_bDehighlightOnRelease(bRestoreOnRelease)
{
    m_img = QPixmap(image1);
    if(!image2.isNull())
        m_imgHighlighted = QPixmap(image2);
}

void ImageButton::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_bDehighlightOnRelease)
        m_bHighlighted = false;

    update();
    emit clicked();
}

void ImageButton::mousePressEvent(QMouseEvent *event)
{
    if(!m_imgHighlighted.isNull())
    {
        m_bHighlighted = !m_bHighlighted;
        update();
    }
}

void ImageButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if(m_imgHighlighted.isNull())
    {
        painter.drawPixmap(rect(), m_img);
    }
    else
    {
        painter.drawPixmap(rect(), m_bHighlighted? m_imgHighlighted : m_img);
    }
}

void ImageButton::updateImage(QString image)
{
    m_img = QPixmap(image);
    update();
}


