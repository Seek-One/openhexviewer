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
	if (m_iListIndex < m_lstPositions.size() - 1) {
		m_iListIndex++;
	}
	// emit
}

void QFindDialogController::findPrevious()
{
	if (m_iListIndex > 0){
		m_iListIndex--;
	}
	// emit 
}

void QFindDialogController::find()
{
	m_lstPositions.empty();
	QString szData = m_pFileViewController->getStringData();
	m_iFindSize = szData.length();

	emit findAllOccurrencesRegex(szData, &m_lstPositions);
	m_iListIndex = 0;

	for(qint64 iTemp : m_lstPositions) {
		qDebug("\t%lld", iTemp);
	}
	qDebug("\n");
}
