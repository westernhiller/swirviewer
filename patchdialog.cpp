#include "patchdialog.h"
#include <QLayout>
#include "mainwindow.h"

PatchDialog::PatchDialog(QWidget *parent)
    : QDialog(parent)
    , m_pSettings(nullptr)
{
    if(parent)
        m_pSettings = static_cast<MainWindow*>(parent)->getSettings();

    setWindowTitle(QString::fromUtf8("截图"));
    setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);

    QImage imgSwir = QImage(tr(":/icons/swir.png"));

    m_pCanvas = new Canvas(this);

    connect(this, SIGNAL(updateImage(QImage)), m_pCanvas, SLOT(updateImage(QImage)));
    emit updateImage(imgSwir);

    QHBoxLayout* pLayout = new QHBoxLayout();
    pLayout->addWidget(m_pCanvas);
    setLayout(pLayout);

#ifdef WIN32
    QSettings settings("HKEY_CURRENT_USER\\Software\\SwirView", QSettings::NativeFormat);
#else
    QString pathDefault = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QSettings settings(pathDefault + "/.swirview.ini", QSettings::NativeFormat);
#endif
    setGeometry(settings.value("patch/geometry").toRect());
}

PatchDialog::~PatchDialog()
{
}

void PatchDialog::resizeEvent(QResizeEvent *event)
{
    m_pCanvas->setGeometry(this->rect());
}

void PatchDialog::keyPressEvent(QKeyEvent* event)
{
    int keyValue = event->key();
    if(keyValue & Qt::Key_Escape)
    {
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
