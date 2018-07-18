/*
 * QWindowMainController.cpp
 *
 *  Created on: 18 july 2018
 *      Author: ebeuque
 */

#include <QCoreApplication>
#include <QAction>

#include "GUI/QAboutDialog.h"
#include "GUI/QWindowMain.h"

#include "QWindowMainController.h"

QWindowMainController::QWindowMainController()
{
	m_pMainWindow = NULL;
}

QWindowMainController::~QWindowMainController()
{

}

void QWindowMainController::init(QWindowMain* pMainWindow)
{
	m_pMainWindow = pMainWindow;

    connect(m_pMainWindow->getAboutAction(), SIGNAL(triggered()), this, SLOT(about()));
    connect(m_pMainWindow->getQuitAction(), SIGNAL(triggered()), qApp, SLOT(quit()));
}

void QWindowMainController::about()
{
	QAboutDialog aboutDialog(m_pMainWindow);
	aboutDialog.exec();
}
