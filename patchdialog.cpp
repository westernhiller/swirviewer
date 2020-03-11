#include "patchdialog.h"
#include <QLayout>

PatchDialog::PatchDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(QString::fromUtf8("截图"));
    setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);

    setGeometry(0, 0, 320, 240);
    QImage imgSwir = QImage(tr(":/icons/swir.png"));

    m_pCanvas = new Canvas(this);

    connect(this, SIGNAL(updateImage(QImage)), m_pCanvas, SLOT(updateImage(QImage)));
    emit updateImage(imgSwir);

    QHBoxLayout* pLayout = new QHBoxLayout();
    pLayout->addWidget(m_pCanvas);
    setLayout(pLayout);
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
