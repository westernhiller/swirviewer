#include "mainwindow.h"
#include <QStandardPaths>
#include <QSettings>
#include <QLayout>
#include "imagebutton.h"
#include "photothread.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_pPatchDlg(nullptr)
    , m_pControl(nullptr)
    , m_nCapturers(1)
    , m_nFrames(0)
    , m_bRecording(false)
    , m_bCapturing(false)
    , m_bAnalyzing(false)
    , m_bCameraConnected(false)
{
    loadConfig();

#ifdef WIN32
    QSettings settings("HKEY_CURRENT_USER\\Software\\SwirView", QSettings::NativeFormat);
#else
    QString pathDefault = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QSettings settings(pathDefault + "/.swirview.ini", QSettings::NativeFormat);
#endif
    restoreGeometry(settings.value("main/geometry").toByteArray());
    restoreState(settings.value("main/windowState").toByteArray());
    QRect geo = geometry();

    QImage imgSwir = QImage(tr(":/icons/swir.png"));
    QColor clearColor;
    clearColor.setHsv(255, 255, 63);

    m_pCanvas = new GLCanvas(this, imgSwir);
    m_pCanvas->setGeometry(210, 0, imgSwir.width(), imgSwir.height());
    m_pCanvas->setClearColor(clearColor);
    connect(m_pCanvas, SIGNAL(clicked()), this, SLOT(showToolbar()));

    m_pToolBar = new QWidget(this);
    m_pToolBar->setGeometry(0, 0, 210, geo.height());
    QVBoxLayout* pToolBar = new QVBoxLayout();
    m_pbtnConnect = new ImageButton(":/icons/connect.png");
    ImageButton* pbtnPhoto = new ImageButton(":/icons/photo.png", ":/icons/photopressed.png");
    ImageButton* pbtnVideo = new ImageButton(":/icons/record.png", ":/icons/stop.png", false);
    m_pbtnConnect->setFixedSize(204, 196);
    pbtnPhoto->setFixedSize(204, 196);
    pbtnVideo->setFixedSize(204, 196);
    pToolBar->addWidget(m_pbtnConnect);
    pToolBar->addWidget(pbtnPhoto);
    pToolBar->addWidget(pbtnVideo);
    m_pToolBar->setLayout(pToolBar);
    m_pToolBar->hide();

    m_pAnalyzer = new Analyzer(this);
    m_pAnalyzer->show();
    m_pControl = new ControlPanel(this);
    m_pControl->show();

    m_pPatchDlg = new PatchDialog(this);
    m_pPatchDlg->show();

    ImageBuffer* pImageBuffer = new ImageBuffer("source", 15, SWIRWIDTH, SWIRHEIGHT, CV_8UC1);
    if(!pImageBuffer)
    {
        qDebug() << "Failed creating image buffer!";
        return;
    }
    m_ImageBufferList.push_back(pImageBuffer);
    pImageBuffer = new ImageBuffer("video", 10, SWIRWIDTH, SWIRHEIGHT, CV_8UC1);
    if(!pImageBuffer)
    {
        qDebug() << "Failed creating video buffer!";
        return;
    }
    m_ImageBufferList.push_back(pImageBuffer);

    for(int i = 0; i < m_nCapturers; i++)
    {
        SwirCapturer* pCapturer = new SwirCapturer(this);
        connect(pCapturer, SIGNAL(getFrame(QByteArray)), this, SLOT(onFrame(QByteArray)));
        if(i == 0)
        {
            connect(pCapturer, SIGNAL(cameraConncted(bool)), this, SLOT(connected(bool)));
            connect(pCapturer, SIGNAL(cameraConncted(bool)), m_pControl, SLOT(connected(bool)));
            connect(pCapturer, SIGNAL(updateMode(uint32_t)), m_pControl, SLOT(setMode(uint32_t)));
            connect(pCapturer, SIGNAL(updateIntegral(double)), m_pControl, SLOT(setIntegral(double)));
            connect(pCapturer, SIGNAL(updateCycle(double)), m_pControl, SLOT(setFrameCycle(double)));
            connect(m_pControl, SIGNAL(updateIntegral(double)), pCapturer, SLOT(setIntegral(double)));
            connect(m_pControl, SIGNAL(updateCycle(double)), pCapturer, SLOT(setCycle(double)));
            connect(m_pControl, SIGNAL(enableHighgain(bool)), pCapturer, SLOT(enableHighgain(bool)));
            connect(m_pControl, SIGNAL(enableNonuniform(bool)), pCapturer, SLOT(enableNonuniformityCorrection(bool)));
            connect(m_pControl, SIGNAL(enableIntegral(bool)), pCapturer, SLOT(enableIntegralAdjustion(bool)));
            connect(m_pControl, SIGNAL(adjustOnsite()), pCapturer, SLOT(adjustOnsite()));
            connect(m_pControl, SIGNAL(enableImageMode()), this, SLOT(setPatchImageMode()));
            connect(m_pControl, SIGNAL(enableImageMode()), m_pPatchDlg, SLOT(enableImageMode()));
        }
        m_CapturerList.push_back(pCapturer);
    }
    m_pDisplayer = new SwirDisplayer(this);
    m_pVideoSaver = new VideoThread(this);
    m_pVideoSaver->start();

    connect(this, SIGNAL(updatePatch(QImage)), m_pPatchDlg, SLOT(updateImage(QImage)));
    connect(this, SIGNAL(updateConnectIcon(QString)), m_pbtnConnect, SLOT(updateImage(QString)));
    connect(this, SIGNAL(updateImage(QImage)), m_pCanvas, SLOT(updateImage(QImage)));
    connect(m_pCanvas, SIGNAL(pixelSelect(QPoint)), this, SLOT(onPixelSelect(QPoint)));
    connect(m_pCanvas, SIGNAL(pixelSelect(QPoint)), m_pControl, SLOT(onPixelSelect(QPoint)));
    connect(this, SIGNAL(pixelSelect(QPoint)), m_pControl, SLOT(onPixelSelect(QPoint)));
    connect(m_pControl, SIGNAL(pixelSelect(QPoint)), this, SLOT(onPixelSelect(QPoint)));
    connect(m_pbtnConnect, SIGNAL(clicked()), this, SLOT(onConnect()));

    connect(this, SIGNAL(updateFPS(float)), m_pControl, SLOT(updateFPS(float)));
    connect(pbtnPhoto, SIGNAL(clicked()), this, SLOT(onSavePhoto()));
    connect(pbtnVideo, SIGNAL(clicked()), this, SLOT(onVideo()));

    connect(this, SIGNAL(updateAnalyzeImage(QByteArray)), m_pAnalyzer, SLOT(updateImage(QByteArray)));

    connect(m_pPatchDlg, SIGNAL(updateSize(int, int)), this, SLOT(updatePatchSize(int, int)));
    connect(m_pDisplayer, SIGNAL(display(QImage)), this, SLOT(onDisplay(QImage)));
    connect(this, SIGNAL(record(bool)), m_pDisplayer, SLOT(onRecord(bool)));
    connect(this, SIGNAL(record(bool)), m_pVideoSaver, SLOT(record(bool)));
    connect(m_pVideoSaver, SIGNAL(videoSaved(QString)), this, SLOT(onVideoSaved(QString)));
    connect(m_pAnalyzer, SIGNAL(imageSaved(QString)), this, SLOT(onPhotoSaved(QString)));

    m_pDisplayer->start();
    QTimer* t = new QTimer(this);
    connect(t, SIGNAL(timeout()), this, SLOT(onTimer()));
    t->start(1000);
}

MainWindow::~MainWindow()
{
    if(m_pVideoSaver)
        delete m_pVideoSaver;
    if(m_pDisplayer)
        delete m_pDisplayer;
    for(int i = 0; i < m_nCapturers; i++)
        delete m_CapturerList[i];
    m_CapturerList.clear();

    for(int i = 0; i < m_ImageBufferList.size(); i++)
        delete m_ImageBufferList[i];
    m_ImageBufferList.clear();
}

void MainWindow::showToolbar()
{
    static bool bToolbar = false;

    bToolbar = !bToolbar;
    if(bToolbar)
        m_pToolBar->show();
    else {
        m_pToolBar->hide();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
#ifdef WIN32
    QSettings settings("HKEY_CURRENT_USER\\Software\\SwirView", QSettings::NativeFormat);
#else
    QString pathDefault = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QSettings settings(pathDefault + "/.swirview.ini", QSettings::NativeFormat);
#endif
    settings.setValue("main/geometry", saveGeometry());
    settings.setValue("main/windowState", saveState()); 

    saveConfig();

    QMainWindow::closeEvent(event);
}

void MainWindow::onConnect()
{
    connectCamera(!m_bCameraConnected);
}

void MainWindow::onVideo()
{
    m_bRecording = !m_bRecording;
    emit record(m_bRecording);
}

void MainWindow::connected(bool bConnected)
{
    if(bConnected)
    {
        emit updateConnectIcon(":/icons/disconnect.png");
    }
    else {
        emit updateConnectIcon(":/icons/connect.png");
    }
    m_bCameraConnected = bConnected;
}

ImageBuffer* MainWindow::getImageBuffer(QString name)
{
    for(int i = 0; i < m_ImageBufferList.size(); i++)
    {
        BUFFERINFO* pBufInfo = m_ImageBufferList[i]->getBufferInfo();
        if(pBufInfo && (pBufInfo->name.compare(name) == 0))
            return m_ImageBufferList[i];
    }
    return nullptr;
}

void MainWindow::onTimer()
{
    static int pos = 0;
    static int framebuffer[] = {0,0,0,0,0,0,0,0,0,0};

    m_mutex.lock();
    framebuffer[pos++] = m_nFrames;
    m_nFrames = 0;
    m_mutex.unlock();

    if(pos == 10)
        pos = 0;

    int nSum = 0;
    for(int i = 0; i < 10; i++)
        nSum += framebuffer[i];
    float fFrame = (float)nSum / 10.0f;

    emit updateFPS(fFrame);

    m_bAnalyzing = true;
    m_bZooming = true;
}

void MainWindow::onDisplay(QImage image)
{
    emit updateImage(image);
    if(m_bZooming)
    {
        emit updatePatch(image.copy(m_patchRect));
        m_bZooming = false;
    }
}


void MainWindow::setPatchImageMode()
{
    QFont font;
    if(m_settings.bImageMode)
    {
        int fw = m_patchRect.width() * TABLE_WIDTH / 4;
        int fh = m_patchRect.height() * TABLE_HEIGHT / 4;
        m_patchRect = QRect(m_patchCenter.x() - fw / 2, m_patchCenter.y() - fh /2, fw, fh);
    }
    else
    {
        int fw = m_patchRect.width() / TABLE_WIDTH * 4;
        int fh = m_patchRect.height() / TABLE_HEIGHT * 4;
        m_patchRect = QRect(m_patchCenter.x() - fw / 2, m_patchCenter.y() - fh /2, fw, fh);
    }
}

void MainWindow::updatePatchSize(int width, int height)
{
    QFont font;
    int fw = width;
    int fh = height;

    if(!m_settings.bImageMode)
    {
        fw = fw / TABLE_WIDTH;
        fh = fh / TABLE_HEIGHT;
    }
    m_patchRect = QRect(m_patchCenter.x() - fw / 2, m_patchCenter.y() - fh /2, fw, fh);
}

void MainWindow::connectCamera(bool bConnect)
{
    for(int i = 0; i < m_nCapturers; i++)
    {
        if(bConnect)
            m_CapturerList[i]->start();
        else
            m_CapturerList[i]->stop();
    }
}

void MainWindow::onSavePhoto()
{
    PhotoThread* pPhotoSaver = new PhotoThread(this);
    connect(pPhotoSaver, SIGNAL(photoSaved(QString)), this, SLOT(onPhotoSaved(QString)));
}

void MainWindow::onPhotoSaved(QString filename)
{
    qDebug() << "Photo " << filename << " saved!";
    PhotoThread* pPhotoSaver = static_cast<PhotoThread*>(sender());
    delete pPhotoSaver;
}

void MainWindow::onVideoSaved(QString filename)
{
    qDebug() << "Video " << filename << " saved!";
}

void MainWindow::onFrame(QByteArray frame)
{
    m_mutex.lock();
    m_nFrames++;
    m_mutex.unlock();

    SwirProcessor* pProcessor = new SwirProcessor(frame, this);
    connect(pProcessor, SIGNAL(killMe()), this, SLOT(killProcessor()));
    pProcessor->start();
    if(m_bAnalyzing)
    {
        m_bAnalyzing = false;
        emit updateAnalyzeImage(frame);
    }
}

void MainWindow::killProcessor()
{
    SwirProcessor* pProcessor = static_cast<SwirProcessor *>(sender());
    delete pProcessor;
}

void MainWindow::onPixelSelect(QPoint pt)
{
    m_patchCenter = pt;
    m_patchRect = QRect(pt.x() - m_patchRect.width()/2,
                        pt.y() - m_patchRect.height()/2,
                        m_patchRect.width(),
                        m_patchRect.height());
//    qDebug() << m_patchRect;
}

void MainWindow::onPatchClose()
{
    if(m_pPatchDlg)
    {
        delete m_pPatchDlg;
        m_pPatchDlg = nullptr;
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    m_pCanvas->setGeometry(this->rect());
}

void MainWindow::loadConfig()
{
    QString pathDefault = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
#ifdef WIN32
    QSettings settings("HKEY_CURRENT_USER\\Software\\SwirView", QSettings::NativeFormat);
#else
    QSettings settings(pathDefault + "/.swirview.ini", QSettings::NativeFormat);
#endif
    settings.beginGroup("swir");
    m_settings.ip = settings.value("ipaddress", "192.168.1.10").toString();
    m_settings.port = settings.value("port", "2001").toInt();
    m_settings.nGain = settings.value("gain", "4").toInt();
    m_settings.bHighgain = settings.value("highgain", "false").toBool();
    m_settings.bSmooth = settings.value("smooth", "false").toBool();
    m_settings.bHistogram = settings.value("histogram", "false").toBool();
    m_settings.bIntegral = settings.value("integral", "false").toBool();
    m_settings.bCorrection = settings.value("correction", "false").toBool();
    settings.endGroup();

    settings.beginGroup("common");
    m_settings.path = settings.value("path", pathDefault).toString();
    m_settings.bMirror = settings.value("mirror", "false").toBool();
    m_settings.bImageMode = settings.value("imagemode", "false").toBool();
    settings.endGroup();

    settings.beginGroup("patch");
    QRect patchRect = settings.value("geometry", "@Rect(300 673 390 401)").toRect();
    m_patchCenter = QPoint(SWIRWIDTH/2, SWIRHEIGHT/2);
    int fw = patchRect.width();
    int fh = patchRect.height();

    if(!m_settings.bImageMode)
    {
        fw = fw / TABLE_WIDTH;
        fh = fh / TABLE_HEIGHT;
    }
    m_patchRect = QRect(SWIRWIDTH/2 - fw/2, SWIRHEIGHT/2 - fh/2, fw, fh);
    settings.endGroup();
}

void MainWindow::saveConfig()
{
#ifdef WIN32
    QSettings settings("HKEY_CURRENT_USER\\Software\\SwirView", QSettings::NativeFormat);
#else
    QString pathDefault = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QSettings settings(pathDefault + "/.swirview.ini", QSettings::NativeFormat);
#endif
    settings.beginGroup("swir");
    settings.setValue("ipaddress", m_settings.ip);
    settings.setValue("port", m_settings.port);
    settings.setValue("gain", m_settings.nGain);
    settings.setValue("highgain", m_settings.bHighgain);
    settings.setValue("smooth", m_settings.bSmooth);
    settings.setValue("histogram", m_settings.bHistogram);
    settings.setValue("integral", m_settings.bIntegral);
    settings.setValue("correction", m_settings.bCorrection);
    settings.endGroup();

    settings.beginGroup("common");
    settings.setValue("path", m_settings.path);
    settings.setValue("mirror", m_settings.bMirror);
    settings.setValue("imagemode", m_settings.bImageMode);
    settings.endGroup();
}

