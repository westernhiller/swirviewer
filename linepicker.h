#ifndef LINEPICKER_H
#define LINEPICKER_H

#include <QWidget>
#include <QCheckBox>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include "global.h"

class LinePicker : public QWidget
{
    Q_OBJECT

public:
    explicit LinePicker(int index = 0, bool bChecked = false, int line = 0, QColor color = Qt::blue, QWidget *parent = nullptr);
    ~LinePicker() override;

    bool getStatus(int* value);
signals:

private:
    QCheckBox* m_pCheckBox;
    QSpinBox*   m_pSpinBox;
    QLabel* m_pColorButton;
};

#endif // LINEPICKER_H
