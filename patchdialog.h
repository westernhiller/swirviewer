#ifndef PATCHDIALOG_H
#define PATCHDIALOG_H

#include <QDialog>
#include <QKeyEvent>
#include <QTextEdit>
#include "canvas.h"

#define TABLE_WIDTH     28.5
#define TABLE_HEIGHT    19.5

class PatchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PatchDialog(QWidget *parent = nullptr);
    ~PatchDialog();

    inline SWIRSETTINGS* getSettings() { return m_pSettings;}
    inline Canvas* getCanvas() { return m_pCanvas;}
signals:
    void updatePatchImage(QImage);
    void updateSize(int, int);
    void movePatchLeft();
    void movePatchRight();
    void movePatchUp();
    void movePatchDown();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    Canvas*         m_pCanvas;
    QTextEdit*      m_pText;
    SWIRSETTINGS*   m_pSettings;

public slots:
    void updateImage(QImage);
    void enableImageMode();
};

#endif // SWIRDIALOG_H
