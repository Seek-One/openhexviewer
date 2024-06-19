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
#include <QStatusBar>

#include "GUI/QFileView.h"
#include "GUI/QFileStructureView.h"
#include "GUI/QBytesView.h"

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

    QWidget* pWidget = new QWidget(pSplitter);
    pSplitter->addWidget(pWidget);

    QVBoxLayout* pVBoxLayout = new QVBoxLayout(pWidget);
    pVBoxLayout->setContentsMargins(0, 0, 0, 0);
    pWidget->setLayout(pVBoxLayout);
    {
        m_pFileStructureView = new QFileStructureView(pWidget);
        pVBoxLayout->addWidget(m_pFileStructureView, 4);
    
        m_pBytesView = new QBytesView(pWidget);
        pVBoxLayout->addWidget(m_pBytesView, 1);
    }

    m_pStatusBar = new QStatusBar(this);
    m_pStatusBar->showMessage("");
    this->setStatusBar(m_pStatusBar);
    
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

    QMenu *pViewMenu = menuBar()->addMenu(tr("&View"));
    m_pGoToAction = new QAction(tr("&Go to Bytes"), this);
    pViewMenu->addAction(m_pGoToAction);
    m_pFindAction = new QAction(tr("&Find"), this);
    pViewMenu->addAction(m_pFindAction);

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

QAction* QWindowMain::getGoToAction() const
{
    return m_pGoToAction;
}

QAction* QWindowMain::getFindAction() const
{
    return m_pFindAction;
}

QFileView* QWindowMain::getFileView() const
{
	return m_pFileView;
}

QFileStructureView* QWindowMain::getFileStructureView() const
{
	return m_pFileStructureView;
}

QBytesView* QWindowMain::getBytesView() const
{
    return m_pBytesView;
}

void QWindowMain::setStatusBarText(const QString& szText)
{
	m_pStatusBar->showMessage(szText);
}
