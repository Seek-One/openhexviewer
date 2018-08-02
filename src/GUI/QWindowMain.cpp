/*
 * MainWindow.cpp
 *
 *  Created on: 18 july 2018
 *      Author: ebeuque
 */

#include <QVBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QSplitter>

#include "GUI/QFileView.h"
#include "GUI/QFileStructureView.h"

#include "QWindowMain.h"

QWindowMain::QWindowMain(QWidget* parent)
	: QMainWindow(parent)
{
    //Set minimum window size
    setMinimumSize(800, 600);

    createMenu();

    QSplitter* pSplitter = new QSplitter(parent);
    pSplitter->setContentsMargins(0, 0, 0, 0);
    setCentralWidget(pSplitter);

    m_pFileView = new QFileView(pSplitter);
    pSplitter->addWidget(m_pFileView);

    m_pFileStructureView = new QFileStructureView(pSplitter);
    pSplitter->addWidget(m_pFileStructureView);

    //QList<int> listSizes;
    //listSizes << 1 << 0;
    //pSplitter->setSizes(listSizes);
}

QWindowMain::~QWindowMain()
{

}

void QWindowMain::createMenu()
{
    QMenu *pFileMenu = menuBar()->addMenu(tr("&File"));
    m_pOpenAction = new QAction(tr("&Open..."), this);
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

QFileView* QWindowMain::getFileView() const
{
	return m_pFileView;
}

QFileStructureView* QWindowMain::getFileStructureView() const
{
	return m_pFileStructureView;
}
