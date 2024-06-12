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
#include "GUI/QWindowMain.h"
#include "GUIController/QFileViewController.h"
#include "GUIController/QFileStructureViewController.h"

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
}

void QWindowMainController::init(QWindowMain* pMainWindow)
{
	m_pMainWindow = pMainWindow;

	connect(m_pMainWindow->getOpenAction(), SIGNAL(triggered()), this, SLOT(openFile()));
	connect(m_pMainWindow->getQuitAction(), SIGNAL(triggered()), qApp, SLOT(quit()));
	connect(m_pMainWindow->getAboutAction(), SIGNAL(triggered()), this, SLOT(about()));

	m_pFileViewController = new QFileViewController(m_pMainWindow->getFileView());

	m_pFileStructureViewController = new QFileStructureViewController(m_pMainWindow->getFileStructureView());

	connect(m_pFileStructureViewController, SIGNAL(fileStructureItemSelected(qint64, qint64)), this, SLOT(selectFileData(qint64, qint64)));

	connect(m_pFileViewController, SIGNAL(onBytesSelectionChanged(qint64, qint64)), this, SLOT(onBytesSelectionChanged(qint64, qint64)));
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

void QWindowMainController::selectFileData(qint64 offset, qint64 size)
{
	m_pFileViewController->selectFileData(offset, size);
}

void QWindowMainController::about()
{
	QAboutDialog aboutDialog(m_pMainWindow);
	aboutDialog.exec();
}

void QWindowMainController::onBytesSelectionChanged(qint64 offset, qint64 size)
{
	QString szTmp;
	if (size > 1) {
		qint64 iOffsetEnd = offset + size - 1;
		QStringASPrintf(szTmp, "Offset: 0x%0llX (%lld), %0llX bytes from 0x%0llX to 0x%0llX (%lld-%lld) selected", iOffsetEnd, iOffsetEnd, size, offset, iOffsetEnd, offset, iOffsetEnd);
	} else {
		QStringASPrintf(szTmp, "Offset: 0x%0llX (%lld)", offset, offset);
	}
	m_pMainWindow->setStatusBarText(szTmp);
}
