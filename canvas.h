#ifndef CANVAS_H
#define CANVAS_H

#include <QLabel>
#include "global.h"

class Canvas : public QLabel
{
    Q_OBJECT

public:
    explicit Canvas(QWidget* parent = nullptr);

protected:
    virtual void paintEvent(QPaintEvent *ev);

protected:
    QPixmap m_img;					/*!<按键图像 */

public slots:
    void updateImage(QImage);
};

#endif // IMAGEBUTTON_H
