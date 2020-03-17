#include "patchdialog.h"
#include <QLayout>
#include "mainwindow.h"

PatchDialog::PatchDialog(QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint)
    , m_pSettings(nullptr)
{
    if(parent)
        m_pSettings = static_cast<MainWindow*>(parent)->getSettings();

    setWindowTitle(QString::fromUtf8("放大"));

    QImage imgSwir = QImage(tr(":/icons/swir.png"));

    m_pCanvas = new Canvas(this);

    connect(this, SIGNAL(updateImage(QImage)), m_pCanvas, SLOT(updateImage(QImage)));
    emit updateImage(imgSwir);

    QHBoxLayout* pLayout = new QHBoxLayout();
    m_pCanvas = new Canvas(this);
    connect(this, SIGNAL(updatePatchImage(QImage)), m_pCanvas, SLOT(updateImage(QImage)));
    pLayout->addWidget(m_pCanvas);
    m_pText = new QTextEdit(this);
    m_pText->setReadOnly(true);
    pLayout->addWidget(m_pText);
    if(m_pSettings && m_pSettings->bImageMode)
        m_pText->hide();
    else
        m_pCanvas->hide();

    setLayout(pLayout);

#ifdef WIN32
    QSettings settings("HKEY_CURRENT_USER\\Software\\SwirView", QSettings::NativeFormat);
#else
    QString pathDefault = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QSettings settings(pathDefault + "/.swirview.ini", QSettings::NativeFormat);
#endif
    setGeometry(settings.value("patch/geometry", "@Rect(300 673 390 401)").toRect());
}

PatchDialog::~PatchDialog()
{
}

void PatchDialog::enableImageMode()
{
    if(m_pSettings && m_pSettings->bImageMode)
    {
        m_pText->hide();
        m_pCanvas->show();
    }
    else
    {
        m_pCanvas->hide();
        m_pText->show();
    }
}

void PatchDialog::updateImage(QImage image)
{
    if(m_pSettings && m_pSettings->bImageMode)
        emit updatePatchImage(image);
    else
    {
        QString text = tr("<table>");
        cv::Mat matImage = qimage2mat(image);

        for(int i = 0; i < image.height(); i++)
        {
            uchar* ptr = matImage.ptr<uchar>(i);
            QString line;
            text += tr("<tr>");
            for(int j = 0; j < image.width(); j++)
            {
                text += QString("<td width=25 height=12 align=\"center\"><font color=");
                if((i + j) % 2 == 0)
                {
                    text += QString("\"blue\"");
                }
                else
                {
                    text += QString("\"red\"");
                }
                text += QString(">%1</font></td>").arg(ptr[j]);//, 4, 10, QChar(' '));
            }
            text += "</tr>";
        }
        text += tr("</table>");
        m_pText->setHtml(text);
    }
}

void PatchDialog::resizeEvent(QResizeEvent *event)
{
    m_pCanvas->setGeometry(this->rect());
    m_pText->setGeometry(this->rect());
    emit updateSize(this->rect().width(), this->rect().height());
}

void PatchDialog::keyPressEvent(QKeyEvent* event)
{
    int keyValue = event->key();
    if(keyValue & Qt::Key_Escape)
    {
    }
    else if(keyValue & Qt::Key_Left)
    {
        emit movePatchLeft();
    }
    else if(keyValue & Qt::Key_Right)
    {
        emit movePatchRight();
    }
    else if(keyValue & Qt::Key_Up)
    {
        emit movePatchUp();
    }
    else if(keyValue & Qt::Key_Down)
    {
        emit movePatchDown();
    }
}

void PatchDialog::closeEvent(QCloseEvent *event)
{
#ifdef WIN32
    QSettings settings("HKEY_CURRENT_USER\\Software\\SwirView", QSettings::NativeFormat);
#else
    QString pathDefault = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QSettings settings(pathDefault + "/.swirview.ini", QSettings::NativeFormat);
#endif
    settings.setValue("patch/geometry", geometry());
    QDialog::closeEvent(event);
}
