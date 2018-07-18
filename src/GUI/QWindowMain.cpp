/*
 * MainWindow.cpp
 *
 *  Created on: 18 july 2018
 *      Author: ebeuque
 */

#include <QVBoxLayout>
#include <QMenu>
#include <QMenuBar>

#include "QWindowMain.h"

QWindowMain::QWindowMain(QWidget* parent)
	: QMainWindow(parent)
{
    //Set minimum window size
    setMinimumSize(800, 600);

    createMenu();
}

QWindowMain::~QWindowMain()
{

}

void QWindowMain::createMenu()
{
    QMenu *pFileMenu = menuBar()->addMenu(tr("&File"));
    m_pOpenAction = new QAction(tr("&Open"), this);
    pFileMenu->addAction(m_pOpenAction);
    pFileMenu->addSeparator();
    m_pQuitAction = new QAction(tr("&Quit"), this);
    pFileMenu->addAction(m_pQuitAction);

    QMenu *pHelpMenu = menuBar()->addMenu(tr("&Help"));
    m_pAboutAction = new QAction(tr("&About"), this);
    pHelpMenu->addAction(m_pAboutAction);
}

QAction* QWindowMain::getOpenAction() const
{
	return m_pOpenAction;
}

QAction* QWindowMain::getQuitAction() const
{
	return m_pQuitAction;
}

QAction* QWindowMain::getAboutAction() const
{
	return m_pAboutAction;
}
