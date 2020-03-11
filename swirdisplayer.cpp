#include "swirdisplayer.h"
#include <QDebug>
#include <QDateTime>

#include "global.h"
#include "mainwindow.h"

using namespace std;
using namespace cv;

SwirDisplayer::SwirDisplayer(QObject* parent)
    : QThread(parent)
{
    MainWindow* pDlg = static_cast<MainWindow*>(parent);
    m_pSettings = pDlg->getSettings();
    m_pImageBuffer = pDlg->getImageBuffer("source");
    m_pVideoBuffer = pDlg->getImageBuffer("video");
}

SwirDisplayer::~SwirDisplayer()
{
    requestInterruption();
    quit();
    wait();
}

void SwirDisplayer::run()
{
    if(!m_pImageBuffer || !m_pVideoBuffer)
        return;

    while (!isInterruptionRequested())
    {
        FRAMEINFO* pFrame = m_pImageBuffer->readImage();
        if(pFrame)
        {
            if(pFrame->matImg.data)
            {
                QImage image = mat2qimage(pFrame->matImg);
                emit display(image);
            }
            if(m_bRecording)
                m_pVideoBuffer->writeImage(*pFrame);
        }
        usleep(10000);
    }
}

void SwirDisplayer::onRecord(bool bRecording)
{
    m_bRecording = bRecording;
}

