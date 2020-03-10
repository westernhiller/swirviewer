#include "patchdialog.h"

PatchDialog::PatchDialog(QWidget *parent)
    : QDialog(parent)
{
    QImage imgSwir = QImage(tr(":/icons/swir.png"));
    QColor clearColor;
    clearColor.setHsv(255, 255, 63);

    m_pCanvas = new GLCanvas(this, imgSwir);
    m_pCanvas->setGeometry(this->rect());
    m_pCanvas->setClearColor(clearColor);

    connect(this, SIGNAL(updateImage(QImage)), m_pCanvas, SLOT(updateImage(QImage)));
}

PatchDialog::~PatchDialog()
{
}

