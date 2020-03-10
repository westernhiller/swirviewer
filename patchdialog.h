#ifndef PATCHDIALOG_H
#define PATCHDIALOG_H

#include <QDialog>
#include "glcanvas.h"

class PatchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PatchDialog(QWidget *parent = nullptr);
    ~PatchDialog();

    inline SWIRSETTINGS* getSettings() { return m_pSettings;}
    inline GLCanvas* getCanvas() { return m_pCanvas;}
signals:

private:
    GLCanvas*       m_pCanvas;
    SWIRSETTINGS*   m_pSettings;
};

#endif // SWIRDIALOG_H
