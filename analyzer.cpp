#include "analyzer.h"
#include <QLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>
#include "mainwindow.h"
#include "linepicker.h"

Analyzer::Analyzer(QWidget* parent)
    : QDialog(parent, Qt::Dialog | Qt::WindowMinimizeButtonHint)
    , m_pSettings(nullptr)
{
    setWindowTitle(QString::fromUtf8("图像分析"));

    if(parent)
        m_pSettings = static_cast<MainWindow*>(parent)->getSettings();

    m_colorlist.push_back(Qt::red);
    m_colorlist.push_back(Qt::yellow);
    m_colorlist.push_back(Qt::white);
    m_colorlist.push_back(Qt::green);
    m_colorlist.push_back(Qt::blue);

    QHBoxLayout* pLayout = new QHBoxLayout();
    QVBoxLayout* pControl = new QVBoxLayout();
    QPushButton* pbtnSnapshot = new QPushButton(QString::fromUtf8("快照"));
    pControl->addWidget(pbtnSnapshot);
    connect(pbtnSnapshot, SIGNAL(clicked()), this, SLOT(dumpImage()));
#ifdef WIN32
//    pbtnSnapshot->setEnabled(false);
#endif
    QVBoxLayout* pTable = new QVBoxLayout();
    for(int i = 0; i < 5; i++)
    {
        LinePicker* pLinePicker = new LinePicker(i, false, SWIRHEIGHT/2, m_colorlist[i]);
        pTable->addWidget(pLinePicker);
        m_linepickerList.append(pLinePicker);
    }

    pControl->addLayout(pTable);
    pLayout->addLayout(pControl);
    createChart(QString::fromUtf8("图像分析"));

    for(int i = 0; i < 5; i++)
    {
        QLineSeries* pSeries = new QLineSeries();
        pSeries->setPen(QPen(m_colorlist[i]));
        m_pChart->addSeries(pSeries);
        pSeries->attachAxis(m_axisX);
        pSeries->attachAxis(m_axisY);
        m_seriesList.push_back(pSeries);
    }

    m_pChartView = new QChartView(m_pChart);
    pLayout->addWidget(m_pChartView);

    setLayout(pLayout);
#ifdef WIN32
    QSettings settings("HKEY_CURRENT_USER\\Software\\SwirView", QSettings::NativeFormat);
#else
    QString pathDefault = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QSettings settings(pathDefault + "/.swirview.ini", QSettings::NativeFormat);
#endif
    setGeometry(settings.value("analyzer/geometry", "@Rect(137 125 615 520)").toRect());
}

Analyzer::~Analyzer()
{

}

void Analyzer::keyPressEvent(QKeyEvent* event)
{
    int keyValue = event->key();
    if(keyValue & Qt::Key_Escape)
    {
    }
}

void Analyzer::closeEvent(QCloseEvent *event)
{
#ifdef WIN32
    QSettings settings("HKEY_CURRENT_USER\\Software\\SwirView", QSettings::NativeFormat);
#else
    QString pathDefault = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QSettings settings(pathDefault + "/.swirview.ini", QSettings::NativeFormat);
#endif
    settings.setValue("analyzer/geometry", geometry());
    QDialog::closeEvent(event);
}

void Analyzer::createChart(QString title)
{
    m_pChart = new QChart();
    m_pChart->legend()->hide();
    m_axisX = new QValueAxis;
    m_axisX->setTickCount(9);
    m_axisX->setMinorTickCount(1);
    m_axisY = new QValueAxis;
    m_axisY->setTickCount(17);
    m_axisY->setMinorTickCount(1);
    m_pChart->addAxis(m_axisX, Qt::AlignBottom);
    m_pChart->addAxis(m_axisY, Qt::AlignLeft);
    m_pChart->setTitle(title);
    m_pChart->setTheme(static_cast<QChart::ChartTheme>(2));

    m_axisX->setLabelFormat("%g");
    m_axisX->setTitleText(tr("X轴"));
    m_axisX->setRange(0, SWIRWIDTH);
    m_axisY->setRange(0, 16384);    // pow(2, 14)
    m_axisY->setTitleText(QString::fromUtf8("灰度值"));
}

void Analyzer::dumpImage()
{
    QImage snapshot = imageFromChart();
    QDateTime timeNow = QDateTime::currentDateTime();
    QString time = timeNow.toString("yyyy-MM-dd-HH-mm-ss-zzz");
    QString path = m_pSettings ? m_pSettings->path : QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString filename = path + tr("/") + time + ".png";

    snapshot.save(filename);
}

QImage Analyzer::imageFromChart()
{
    QScreen* screen = QGuiApplication::primaryScreen();
    // same coordinate as geometry at line 196
    QRect rt = m_pChartView->geometry();
    QPixmap p = screen->grabWindow(winId(), rt.x(), rt.y(), rt.width(), rt.height());
    return p.toImage();
}

void Analyzer::updateImage(QByteArray image)
{
    memcpy(&m_packageBuffer, image.data(), FRAMEBUFSIZE);
    uint16_t *buffer = m_packageBuffer.image;

    for(int i = 0; i < 5; i++)
    {
        QVector<QPointF > testData;
        int value = -1;
        if(m_linepickerList[i]->getStatus(&value))
        {
            uint16_t* ptr = buffer + value * SWIRWIDTH;
            for(int j = 0; j < SWIRWIDTH; j++)
            {
                testData.push_back(QPointF(j, ptr[j]));
            }
        }
        m_seriesList[i]->replace(testData);
    }
}
