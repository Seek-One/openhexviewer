#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "QGoToBytesController.h"
#include "GUI/QGoToBytes.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QIcon>
#include <QToolButton>
#include <QPushButton>

#include <QFile>


QGoToBytesController::QGoToBytesController(QGoToBytes* pGoToBytes)
{
	m_pGoToBytes = pGoToBytes;

    connect(m_pGoToBytes, SIGNAL(accepted()), this, SLOT(researchOffSet()));
}

QGoToBytesController::~QGoToBytesController()
{

}

void QGoToBytesController::researchOffSet()
{
    QString szText = m_pGoToBytes->getLineOffset();
    bool bOk;
    
    if (szText.isEmpty()) {
        QMessageBox::critical(this, tr("Problem"), tr("No Offset has been specified"), QMessageBox::Ok, QMessageBox::Ok);
        return; 
    }
    int iText = szText.toInt(&bOk, m_pGoToBytes->getLineBase());
    if (bOk) {
        return emit changeOffset(iText, 1);
    }
    QMessageBox::critical(this, tr("Problem"), tr("You may only give the offset as : \n\t a hex number or a positive decimal number"), QMessageBox::Ok, QMessageBox::Ok);
}