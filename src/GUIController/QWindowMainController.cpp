/*
 * QWindowMainController.cpp
 *
 *  Created on: 18 july 2018
 *      Author: ebeuque
 */

#include <QCoreApplication>
#include <QAction>
#include <QFileDialog>

#include "GUI/QAboutDialog.h"
#include "GUI/QWindowMain.h"
#include "GUIController/QFileViewController.h"

#include "QWindowMainController.h"

QWindowMainController::QWindowMainController()
{
	m_pMainWindow = NULL;
	m_pFileViewController = NULL;
}

QWindowMainController::~QWindowMainController()
{
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
}

void QWindowMainController::about()
{
	QAboutDialog aboutDialog(m_pMainWindow);
	aboutDialog.exec();
}
