#ifndef IMAGEBUTTON_H
#define IMAGEBUTTON_H

#include <QLabel>
#include "global.h"

class ImageButton : public QLabel
{
    Q_OBJECT

public:
    explicit ImageButton(QString image1, QString image2 = "", bool bRestoreOnReplease = true, QWidget* parent = nullptr);

    inline bool isHighlighted() { return m_bHighlighted;}
signals:
    void clicked();

protected:
    virtual void mousePressEvent(QMouseEvent *ev);
    virtual void mouseReleaseEvent(QMouseEvent *ev);
    virtual void paintEvent(QPaintEvent *ev);

protected:
    bool    m_bHighlighted;			/*!<表示按键高亮状态的变量 */
    bool    m_bDehighlightOnRelease;/*!<表示是否案件松开取消高亮的变量 */

    QPixmap m_img;					/*!<按键图像 */
    QPixmap m_imgHighlighted;		/*!<按键按下时图像 */
};

#endif // IMAGEBUTTON_H
