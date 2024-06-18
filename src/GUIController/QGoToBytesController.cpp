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
    int iRadio = m_pGoToBytes->getRadioButton();
    QString szText = m_pGoToBytes->getLineOffset()->text();
    bool bOk;
    
    if (szText.isEmpty()) {
        return messageBox("No Offset has been specified");
    }

    if (iRadio == 1) { //HEX
        int iText = szText.toInt(&bOk, 16);
        if (bOk) {
            emit changeOffset(iText, 1);
            return;
        }
        return messageBox("You may only give the offset as : \n\t a hex number");
    } else if (iRadio == 2) { //DEC
        int iText = szText.toInt(&bOk, 10);
        if (bOk) {
            emit changeOffset(iText, 1);
            return;
        }
        return messageBox("You may only give the offset as : \n\t a positive decimal number");
    }
    return messageBox("Radio button problem");
}

void QGoToBytesController::messageBox(QString szText)
{
	QMessageBox::critical(this, "", szText, QMessageBox::Ok, QMessageBox::Ok);
}