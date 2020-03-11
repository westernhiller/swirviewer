#ifndef PATCHDIALOG_H
#define PATCHDIALOG_H

#include <QDialog>
#include <QKeyEvent>
#include "canvas.h"

class PatchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PatchDialog(QWidget *parent = nullptr);
    ~PatchDialog();

    inline SWIRSETTINGS* getSettings() { return m_pSettings;}
    inline Canvas* getCanvas() { return m_pCanvas;}
signals:
    void updateImage(QImage);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    Canvas*         m_pCanvas;
    SWIRSETTINGS*   m_pSettings;
};

#endif // SWIRDIALOG_H
