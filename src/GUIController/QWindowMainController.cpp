/*
 * QWindowMainController.cpp
 *
 *  Created on: 18 july 2018
 *      Author: ebeuque
 */

#include <QCoreApplication>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QWidget>

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
	m_pFileExportViewController = NULL;
	m_windowPreferences = NULL;
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

	if (m_pFileExportViewController) {
		delete m_pFileExportViewController;
		m_pFileExportViewController = NULL;
	}
}

void QWindowMainController::init(QWindowMain* pMainWindow)
{
	m_pMainWindow = pMainWindow;

	connect(m_pMainWindow, SIGNAL(openFileClicked()), this, SLOT(openFile()));
	connect(m_pMainWindow, SIGNAL(preferencesClicked()), this, SLOT(preferences()));
	connect(m_pMainWindow, SIGNAL(saveFileClicked()), this, SLOT(saveFile()));
	connect(m_pMainWindow, SIGNAL(quitClicked()), qApp, SLOT(quit()));
	connect(m_pMainWindow, SIGNAL(aboutClicked()), this, SLOT(about()));
	connect(m_pMainWindow, SIGNAL(goToClicked()), this, SLOT(goToBytes()));
	connect(m_pMainWindow, SIGNAL(findClicked()), this, SLOT(find()));
	connect(m_pMainWindow, SIGNAL(colorClicked()), this, SLOT(color()));
	connect(m_pMainWindow, SIGNAL(exportSelectionClicked()), this, SLOT(exportSelection()));

	connect(m_pMainWindow, SIGNAL(mainWindowClosed(QCloseEvent*)), this, SLOT(close(QCloseEvent*)));

	m_pFileViewController = new QFileViewController(m_pMainWindow->getFileView());

	m_pFileStructureViewController = new QFileStructureViewController(m_pMainWindow->getFileStructureView());

	m_pBytesViewController = new QBytesViewController(m_pMainWindow->getBytesView());

	connect(m_pFileStructureViewController, SIGNAL(fileStructureItemSelected(qint64, qint64)), this, SLOT(selectFileData(qint64, qint64)));

	connect(m_pFileViewController, SIGNAL(onBytesSelectionChanged(qint64, qint64)), this, SLOT(onBytesSelectionChanged(qint64, qint64)));

	connect(m_pFileViewController, SIGNAL(onBytesChanged(QString)), m_pBytesViewController, SLOT(handleBytesChanged(QString)));

	connect(m_pFileViewController, &QFileViewController::fileOpened, this, [this]() {
    	m_pMainWindow->actionFileUsable(true);
	});
	connect(m_pFileViewController, &QFileViewController::fileClosed, this, [this]() {
    	m_pMainWindow->actionFileUsable(false);
	});

	connect(m_pFileViewController, SIGNAL(doChanges()), this, SLOT(doChanges()));

	connect(this, SIGNAL(colorText(bool)), m_pFileViewController, SLOT(colorText(bool)));

	connect(m_pMainWindow, SIGNAL(undoClicked()), m_pFileViewController, SLOT(undo()));
	
    m_bSavedChanges = true;

	m_pMainWindow->actionFileUsable(false);
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
	m_bSavedChanges = true;
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
	connect(m_pFindDialogController, SIGNAL(findAllOccurrencesRegex(QByteArray, QList<qint64>*, qint64, qint64)), m_pFileViewController, SLOT(findAllOccurrencesRegex(QByteArray, QList<qint64>*, qint64, qint64)));
	connect(m_pFindDialogController, SIGNAL(selectData(qint64, qint64)), this, SLOT(selectFileData(qint64, qint64)));
	findDialog.exec();
}

void QWindowMainController::onBytesSelectionChanged(qint64 offset, qint64 size)
{
	QString szTmp;
	if (size > 1) {
		qint64 iOffsetEnd = offset + size - 1;
		szTmp = tr("Offset: 0x%1 (%2), %3 bytes from 0x%4 to 0x%5 (%6-%7) selected")
			.arg(iOffsetEnd, 0, 16)
			.arg(iOffsetEnd)
			.arg(size)
			.arg(offset, 0, 16)
			.arg(iOffsetEnd, 0, 16)
			.arg(offset)
			.arg(iOffsetEnd);
	} else {
		szTmp = tr("Offset: 0x%1 (%2)")
			.arg(offset, 0, 16)
			.arg(offset);
	}
	m_pMainWindow->setStatusBarText(szTmp);
}

void QWindowMainController::preferences()
{
	m_windowPreferences = new QWindowPreferences(NULL);
	m_pWindowPreferencesController = new QWindowPreferencesController(m_windowPreferences);
	connect(m_pWindowPreferencesController->getPreferencesFilesStructuresViewController(), &QPreferencesFilesStructuresViewController::updateFile, m_pFileStructureViewController, &QFileStructureViewController::reload);
	m_windowPreferences->setAttribute(Qt::WA_DeleteOnClose);
	m_windowPreferences->show();
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
	emit colorText(m_pMainWindow->getColorIsChecked());
}

void QWindowMainController::close(QCloseEvent* event)
{
	if (!m_bSavedChanges) {
		QMessageBox::StandardButton resBtn = QMessageBox::question(new QWidget(), tr("Confirm Close"),
				tr("You have unsaved changes. Do you want to save them?\n"),
				QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
				QMessageBox::Save);
		if (resBtn == QMessageBox::Save) {
			m_pFileViewController->saveFile();
			event->accept();
			if (m_windowPreferences != NULL) {
				m_windowPreferences->close();
			}
		} else if (resBtn == QMessageBox::Discard) {
			event->accept();
			if (m_windowPreferences != NULL) {
				m_windowPreferences->close();
			}
		} else {
			event->ignore();
		}
	} else {
		event->accept();
		if (m_windowPreferences != NULL) {
			m_windowPreferences->close();
		}
	}
}

void QWindowMainController::doChanges()
{
	m_bSavedChanges = false;
}