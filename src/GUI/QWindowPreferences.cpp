#include <QVBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QSplitter>
#include <QStatusBar>
#include <QTabWidget>
#include <QWidget>

#include "QWindowPreferences.h"
#include "QPreferencesFilesStructuresView.h"

QWindowPreferences::QWindowPreferences(QWidget* parent)
	: QMainWindow(parent)
{
    QSplitter* pSplitter = new QSplitter(parent);
    pSplitter->setContentsMargins(0, 0, 0, 0);
    setCentralWidget(pSplitter);

    QTabWidget* pTabWidget = new QTabWidget(pSplitter);
    pSplitter->addWidget(pTabWidget);
    {
        QSplitter* pSplitter2 = new QSplitter(parent);
        pTabWidget->addTab(pSplitter2, tr("&Structure Files"));
        {
            m_pPreferencesFilesStructuresView = new QPreferencesFilesStructuresView();
            pSplitter2->addWidget(m_pPreferencesFilesStructuresView);


            QWidget* pWidget = new QWidget(pSplitter2);
            pSplitter2->addWidget(pWidget);

            QVBoxLayout* pVBoxLayout = new QVBoxLayout();
            pWidget->setLayout(pVBoxLayout);
            {
                pVBoxLayout->addWidget(new QWidget(), 6);
                m_pAddButton = new QPushButton("add");
                pVBoxLayout->addWidget(m_pAddButton, 1);
                m_pRemoveButton = new QPushButton("add");
                pVBoxLayout->addWidget(m_pRemoveButton, 1);
            }
            
        }
        pTabWidget->addTab(new QWidget(), "test");
        pTabWidget->addTab(new QWidget(), "test2");
    }
        // pMainLayout->addWidget(pTabWidget);
            // pButtonLayout->addWidget(m_pRemoveButton);
        // }
    // }

    // createStatusBar();
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
    // return NULL;
}
