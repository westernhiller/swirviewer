#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "glcanvas.h"
#include "imagebutton.h"
#include "patchdialog.h"
#include "analyzer.h"
#include "controlpanel.h"
#include "swircapturer.h"
#include "swirprocessor.h"
#include "videothread.h"
#include "swirdisplayer.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    inline SWIRSETTINGS* getSettings() { return &m_settings;}
    ImageBuffer* getImageBuffer(QString name);

signals:
    void updateImage(QImage);
    void updatePatch(QImage);
    void pixelSelect(QPoint);
    void updateConnectIcon(QString);
    void updateAnalyzeImage(QByteArray);
    void saveVideoFrame(QImage);
    void updateFPS(float);
    void record(bool);

protected:
    void resizeEvent(QResizeEvent * event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    QWidget*        m_pToolBar;
    GLCanvas*       m_pCanvas;
    SWIRSETTINGS    m_settings;
    ImageButton*    m_pbtnConnect;
    PatchDialog*    m_pPatchDlg;
    QRect           m_patchRect;
    QPoint          m_patchCenter;
    Analyzer*       m_pAnalyzer;
    ControlPanel*   m_pControl;
    int             m_nCapturers;
    QList<SwirCapturer*> m_CapturerList;
    VideoThread*    m_pVideoSaver;
    SwirDisplayer*  m_pDisplayer;
    QList<ImageBuffer*> m_ImageBufferList;
    QMutex          m_mutex;
    int             m_nFrames;
    bool            m_bRecording;
    bool            m_bCapturing;
    bool            m_bAnalyzing;
    bool            m_bZooming;
    bool            m_bCameraConnected;

    void            loadConfig();
    void            saveConfig();
    void            connectCamera(bool);

public slots:
    void            updatePatchSize(int width, int height);
    void            onPixelSelect(QPoint);
    void            onPatchClose();
    void            onFrame(QByteArray);
    void            onDisplay(QImage);
    void            onTimer();
    void            killProcessor();
    void            onSavePhoto();
    void            onVideo();
    void            onPhotoSaved(QString);
    void            onVideoSaved(QString);
    void            onConnect();
    void            connected(bool);
    void            showToolbar();
    void            setPatchImageMode();
};

#endif // MAINWINDOW_H
