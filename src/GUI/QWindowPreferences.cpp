#include <QSplitter>
#include <QStatusBar>
#include <QTabWidget>
#include <QWidget>

#include "QWindowPreferences.h"
#include "QPreferencesFilesStructuresView.h"

QWindowPreferences::QWindowPreferences(QWidget* parent)
	: QMainWindow(parent)
{
    setMinimumSize(400, 300);
    QSplitter* pSplitter = new QSplitter(parent);
    pSplitter->setContentsMargins(0, 0, 0, 0);
    setCentralWidget(pSplitter);

    QTabWidget* pTabWidget = new QTabWidget(pSplitter);
    pSplitter->addWidget(pTabWidget);
    {
        m_pPreferencesFilesStructuresView = new QPreferencesFilesStructuresView(this);
        pTabWidget->addTab(m_pPreferencesFilesStructuresView, tr("&Structure Files"));
        pTabWidget->addTab(new QWidget(), tr("&Other"));
    }
    createStatusBar();
}

QWindowPreferences::~QWindowPreferences()
{

}

void QWindowPreferences::createStatusBar()
{
    m_pStatusBar = new QStatusBar(this);
    m_pStatusBar->showMessage("");
    this->setStatusBar(m_pStatusBar);
}

QPreferencesFilesStructuresView* QWindowPreferences::getFileStructureView() const
{
    return m_pPreferencesFilesStructuresView;
}

void QWindowPreferences::changedPreferencesStatusBar(QString szText)
{
    m_pStatusBar->showMessage(szText);
}