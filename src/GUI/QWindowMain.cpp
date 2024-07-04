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
#include "GUI/QWindowPreferences.h"

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
    {
        QAction* pOpenAction = new QAction(tr("&Open..."), this);
        pOpenAction->setShortcut(QKeySequence("Ctrl+O"));
        connect(pOpenAction, &QAction::triggered, [this]() {
            emit openFileClicked();
        });
        pFileMenu->addAction(pOpenAction);

        pFileMenu->addSeparator();

        QAction* pPreferencesAction = new QAction(tr("&Preferences"), this);
        connect(pPreferencesAction, &QAction::triggered, [this]() {
            emit preferencesClicked();
        });
        pFileMenu->addAction(pPreferencesAction);

        pFileMenu->addSeparator();

        m_pSaveAction = new QAction(tr("&Save"), this);
        m_pSaveAction->setShortcut(QKeySequence("Ctrl+S"));
        connect(m_pSaveAction, &QAction::triggered, [this]() {
            emit saveFileClicked();
        });
        pFileMenu->addAction(m_pSaveAction);

        pFileMenu->addSeparator();

        QAction* pQuitAction = new QAction(tr("&Quit"), this);
        pQuitAction->setShortcut(QKeySequence("Ctrl+Q"));
        connect(pQuitAction, &QAction::triggered, [this]() {
            emit quitClicked();
        });
        pFileMenu->addAction(pQuitAction);
    }
    
    QMenu *pEditMenu = menuBar()->addMenu(tr("&Edit"));
    {
        m_pGoToAction = new QAction(tr("&Go to"), this);
        m_pGoToAction->setShortcut(QKeySequence("Ctrl+J"));
        connect(m_pGoToAction, &QAction::triggered, [this]() {
            emit goToClicked();
        });
        pEditMenu->addAction(m_pGoToAction);
        m_pFindAction = new QAction(tr("&Find"), this);
        m_pFindAction->setShortcut(QKeySequence("Ctrl+F"));
        pEditMenu->addAction(m_pFindAction);
        connect(m_pFindAction, &QAction::triggered, [this]() {
            emit findClicked();
        });

        pEditMenu->addSeparator();

        m_pExportSelectionAction = new QAction(tr("&Export Selection"), this);
        m_pExportSelectionAction->setShortcut(QKeySequence("Ctrl+E"));
        connect(m_pExportSelectionAction, &QAction::triggered, [this]() {
            emit exportSelectionClicked();
        });
        pEditMenu->addAction(m_pExportSelectionAction);
    }

    QMenu *pViewMenu = menuBar()->addMenu(tr("&View"));
    {
        m_pColorAction = new QAction(tr("&Colorize"), this);
        m_pColorAction->setShortcut(QKeySequence("Ctrl+K"));
        connect(m_pColorAction, &QAction::triggered, [this]() {
            emit colorClicked();
        });
        m_pColorAction->setCheckable(true);
        pViewMenu->addAction(m_pColorAction);
    }

    QMenu *pHelpMenu = menuBar()->addMenu(tr("&Help"));
    {
        QAction* pAboutAction = new QAction(tr("&About"), this);
        pAboutAction->setShortcut(QKeySequence("Ctrl+H"));
        connect(pAboutAction, &QAction::triggered, [this]() {
            emit aboutClicked();
        });
        pHelpMenu->addAction(pAboutAction);
    }
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

void QWindowMain::closeEvent(QCloseEvent* event)
{
    emit mainWindowClosed(event);
}

void QWindowMain::actionFileUsable(bool bEnabled)
{
    m_pSaveAction->setEnabled(bEnabled);
    m_pGoToAction->setEnabled(bEnabled);
    m_pFindAction->setEnabled(bEnabled);
    m_pExportSelectionAction->setEnabled(bEnabled);
}

bool QWindowMain::getColorIsChecked()
{
    return m_pColorAction->isChecked();
}