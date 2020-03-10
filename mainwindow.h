#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "glcanvas.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    inline SWIRSETTINGS* getSettings() { return &m_settings;}
signals:
    void updateImage(QImage);

protected:
    void resizeEvent(QResizeEvent * event) override;

private:
    GLCanvas*       m_pCanvas;
    SWIRSETTINGS    m_settings;

    void            loadConfig();
    void            saveConfig();
};

#endif // MAINWINDOW_H
