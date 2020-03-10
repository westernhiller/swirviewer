#include "mainwindow.h"
#include <QStandardPaths>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    loadConfig();

    QImage imgSwir = QImage(tr(":/icons/swir.png"));
    QColor clearColor;
    clearColor.setHsv(255, 255, 63);

    m_pCanvas = new GLCanvas(this, imgSwir);
    m_pCanvas->setGeometry(this->rect());
    m_pCanvas->setClearColor(clearColor);

    connect(this, SIGNAL(updateImage(QImage)), m_pCanvas, SLOT(updateImage(QImage)));
}

MainWindow::~MainWindow()
{

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
    m_settings.bKeepRatio = settings.value("keepratio", "false").toBool();
    m_settings.bMirror = settings.value("mirror", "false").toBool();

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

    settings.beginGroup("Common");
    settings.setValue("path", m_settings.path);
    settings.setValue("keepratio", m_settings.bKeepRatio);
    settings.setValue("mirror", m_settings.bMirror);
    settings.endGroup();
}

