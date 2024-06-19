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
	qDebug("findNext");

}

void QFindDialogController::findPrevious()
{
	qDebug("findPrevious");
	
}

void QFindDialogController::find()
{
	qDebug("find");
}
