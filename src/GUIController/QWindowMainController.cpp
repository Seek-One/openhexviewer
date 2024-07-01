/*
 * QWindowMainController.cpp
 *
 *  Created on: 18 july 2018
 *      Author: ebeuque
 */

#include <QCoreApplication>
#include <QAction>
#include <QFileDialog>

#include "Global/QtCompat.h"

#include "GUI/QAboutDialog.h"
#include "GUI/QGoToBytes.h"
#include "GUI/QFindDialog.h"
#include "GUI/QWindowMain.h"
#include "GUI/QWindowPreferences.h"
#include "GUI/QFileExportView.h"
#include "GUIController/QFileViewController.h"
#include "GUIController/QFileStructureViewController.h"
#include "GUIController/QBytesViewController.h"
#include "GUIController/QGoToBytesController.h"
#include "GUIController/QFindDialogController.h"
#include "GUIController/QWindowPreferencesController.h"
#include "GUIController/QPreferencesFilesStructuresViewController.h"
#include "GUIController/QFileExportViewController.h"

#include "QWindowMainController.h"

QWindowMainController::QWindowMainController()
{
	m_pMainWindow = NULL;
	m_pFileViewController = NULL;
	m_pFileStructureViewController = NULL;
}

QWindowMainController::~QWindowMainController()
{
	if(m_pFileStructureViewController){
		delete m_pFileStructureViewController;
		m_pFileStructureViewController = NULL;
	}
	if(m_pFileViewController){
		delete m_pFileViewController;
		m_pFileViewController = NULL;
	}

	if(m_pBytesViewController) {
		delete m_pBytesViewController;
		m_pBytesViewController = NULL;
	}

	// if (m_pFileExportViewController) {
	// 	delete m_pFileExportViewController;
	// 	m_pFileExportViewController = NULL;
	// }
}

void QWindowMainController::init(QWindowMain* pMainWindow)
{
	m_pMainWindow = pMainWindow;

	connect(m_pMainWindow->getOpenAction(), SIGNAL(triggered()), this, SLOT(openFile()));
	connect(m_pMainWindow->getPreferencesAction(), SIGNAL(triggered()), this, SLOT(preferences()));
	connect(m_pMainWindow->getSaveAction(), SIGNAL(triggered()), this, SLOT(saveFile()));
	connect(m_pMainWindow->getQuitAction(), SIGNAL(triggered()), qApp, SLOT(quit()));
	connect(m_pMainWindow->getAboutAction(), SIGNAL(triggered()), this, SLOT(about()));
	connect(m_pMainWindow->getGoToAction(), SIGNAL(triggered()), this, SLOT(goToBytes()));
	connect(m_pMainWindow->getFindAction(), SIGNAL(triggered()), this, SLOT(find()));
	connect(m_pMainWindow->getColorAction(), SIGNAL(triggered()), this, SLOT(color()));
	connect(m_pMainWindow->getExportSelectionAction(), SIGNAL(triggered()), this, SLOT(exportSelection()));
	actionDisabled();

	m_pFileViewController = new QFileViewController(m_pMainWindow->getFileView());

	m_pFileStructureViewController = new QFileStructureViewController(m_pMainWindow->getFileStructureView());

	m_pBytesViewController = new QBytesViewController(m_pMainWindow->getBytesView());

	connect(m_pFileStructureViewController, SIGNAL(fileStructureItemSelected(qint64, qint64)), this, SLOT(selectFileData(qint64, qint64)));

	connect(m_pFileViewController, SIGNAL(onBytesSelectionChanged(qint64, qint64)), this, SLOT(onBytesSelectionChanged(qint64, qint64)));

	connect(m_pFileViewController, SIGNAL(onBytesChanged(QString)), m_pBytesViewController, SLOT(handleBytesChanged(QString)));

	connect(m_pFileViewController, SIGNAL(fileOpened()), this, SLOT(actionEnabled()));

	connect(m_pFileViewController, SIGNAL(fileClosed()), this, SLOT(actionDisabled()));

	connect(this, SIGNAL(colorText(bool)), m_pFileViewController, SLOT(colorText(bool)));
}

void QWindowMainController::openFile()
{
	QFileDialog dialog(m_pMainWindow);
	dialog.setFileMode(QFileDialog::AnyFile);
	dialog.setViewMode(QFileDialog::Detail);
	if (dialog.exec()){
		QStringList listSelectedFiles;
		listSelectedFiles = dialog.selectedFiles();

	    QStringList::const_iterator iter;
	    for(iter = listSelectedFiles.constBegin(); iter != listSelectedFiles.constEnd(); ++iter)
	    {
	    	openFile(*iter);
	    }
	}
}

void QWindowMainController::openFile(const QString& szFilePath)
{
	qDebug("[Main] Opening file %s", qPrintable(szFilePath));
	m_pFileViewController->openFile(szFilePath);
	m_pFileStructureViewController->setCurrentFile(szFilePath);
}

void QWindowMainController::saveFile()
{
	qDebug("[Main] Saving file");
	m_pFileViewController->saveFile();
}

void QWindowMainController::selectFileData(qint64 offset, qint64 size)
{
	m_pFileViewController->selectFileData(offset, size);
}

void QWindowMainController::about()
{
	QAboutDialog aboutDialog(m_pMainWindow);
	aboutDialog.exec();
}

void QWindowMainController::goToBytes()
{
	QGoToBytes goTo(m_pMainWindow);
	m_pGoToBytesController = new QGoToBytesController(&goTo);
	connect(m_pGoToBytesController, SIGNAL(changeOffset(qint64, qint64)), this, SLOT(selectFileData(qint64, qint64)));
	goTo.exec();
}

void QWindowMainController::find()
{
	QFindDialog findDialog(m_pMainWindow);
	m_pFindDialogController = new QFindDialogController(&findDialog);
	connect(m_pFindDialogController, SIGNAL(findAllOccurrencesRegex(QString, QList<qint64>*)), m_pFileViewController, SLOT(findAllOccurrencesRegex(QString, QList<qint64>*)));
	connect(m_pFindDialogController, SIGNAL(selectData(qint64, qint64)), this, SLOT(selectFileData(qint64, qint64)));
	findDialog.exec();
}


void QWindowMainController::onBytesSelectionChanged(qint64 offset, qint64 size)
{
	QString szTmp;
	if (size > 1) {
		qint64 iOffsetEnd = offset + size - 1;
		//TODO tr
		QStringASPrintf(szTmp, "Offset: 0x%0llX (%lld), %lld bytes from 0x%0llX to 0x%0llX (%lld-%lld) selected", iOffsetEnd, iOffsetEnd, size, offset, iOffsetEnd, offset, iOffsetEnd);
	} else {
		//TODO tr
		QStringASPrintf(szTmp, "Offset: 0x%0llX (%lld)", offset, offset);
	}
	m_pMainWindow->setStatusBarText(szTmp);
}

void QWindowMainController::preferences()
{
	QWindowPreferences* windowPreferences = new QWindowPreferences(NULL);
	m_pWindowPreferencesController = new QWindowPreferencesController(windowPreferences);
	connect(m_pWindowPreferencesController->getPreferencesFilesStructuresViewController(), &QPreferencesFilesStructuresViewController::updateFile, m_pFileStructureViewController, &QFileStructureViewController::reload);
	windowPreferences->setAttribute(Qt::WA_DeleteOnClose);
	windowPreferences->show();
}

void QWindowMainController::exportSelection()
{
	QFileExportView* pfileExportView = new QFileExportView(NULL);
	m_pFileExportViewController = new QFileExportViewController(pfileExportView);
	connect(m_pFileExportViewController, SIGNAL(changeOffset(qint64, qint64)), this, SLOT(selectFileData(qint64, qint64)));
	connect(m_pFileExportViewController, SIGNAL(selection(QString&)), m_pFileViewController, SLOT(selection(QString&)));
	qint64 offset;
	qint64 size;
	m_pFileViewController->getSelectionOffset(offset, size);
	m_pFileExportViewController->init(offset, size);
	pfileExportView->exec();
}

void QWindowMainController::color()
{
	emit colorText(m_pMainWindow->getColorAction()->isChecked());
}

void QWindowMainController::actionUsable(bool bEnabled)
{
	m_pMainWindow->getGoToAction()->setEnabled(bEnabled);
	m_pMainWindow->getSaveAction()->setEnabled(bEnabled);
	m_pMainWindow->getFindAction()->setEnabled(bEnabled);
	m_pMainWindow->getExportSelectionAction()->setEnabled(bEnabled);
}

void QWindowMainController::actionEnabled() 
{
	actionUsable(true);
}

void QWindowMainController::actionDisabled()
{
	actionUsable(false);
}
	