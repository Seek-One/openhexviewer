#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Global/QtCompat.h"
#include "QFindDialogController.h"
#include "../GUI/QFileView.h"
#include "QFileViewController.h" // Include the full definition


#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QIcon>
#include <QToolButton>
#include <QPushButton>
#include <QPlainTextEdit>

#include <QFile>


QFindDialogController::QFindDialogController(QFindDialog* pFindDialog)
{
	m_pFindDialog = pFindDialog;
	m_pFileViewController = new QFileViewController(m_pFindDialog->getFileView());
	

	connect(m_pFindDialog, SIGNAL(accepted()), this, SLOT(find()));
	connect(m_pFindDialog, SIGNAL(findNext()), this, SLOT(findNext()));
	connect(m_pFindDialog, SIGNAL(findPrevious()), this, SLOT(findPrevious()));
}

QFindDialogController::~QFindDialogController()
{

}

void QFindDialogController::findNext()
{
	if (m_lstPositions.isEmpty() || m_iPositionsSize <= 0) {
		m_pFindDialog->setLabelNbOcc(-1, -1);
		return;
	}
	(++m_iListIndex) %= m_iPositionsSize;
	m_pFindDialog->setLabelNbOcc(m_iListIndex + 1, m_iPositionsSize);
	
	emit selectData(m_lstPositions.at(m_iListIndex), m_pFileViewController->getStringData().length());
}

void QFindDialogController::findPrevious()
{
	if (m_lstPositions.isEmpty() || m_iPositionsSize <= 0) {
		m_pFindDialog->setLabelNbOcc(-1, -1);
		return;
	}
	if (--m_iListIndex < 0) {
		m_iListIndex = m_iPositionsSize - 1;
	}
	m_pFindDialog->setLabelNbOcc(m_iListIndex + 1, m_iPositionsSize);

	emit selectData(m_lstPositions.at(m_iListIndex), m_pFileViewController->getStringData().length());
}

void QFindDialogController::find()
{
	m_lstPositions.clear();
	QString szData = m_pFileViewController->getStringData();
	m_iFindSize = szData.length();

	emit findAllOccurrencesRegex(szData, &m_lstPositions);
	m_iListIndex = 0;
	m_iPositionsSize = m_lstPositions.size(); 
	if (m_iPositionsSize > 0) {
		m_pFindDialog->setLabelNbOcc(m_iListIndex + 1, m_iPositionsSize);
	} else {
		m_pFindDialog->setLabelNbOcc(-1, -1);
	}
}
