#include <limits>
#include <math.h>

#include <QObject>
#include <QListWidget>
#include <QSplitter>
#include <QVBoxLayout>

#include "QPreferencesFilesStructuresView.h"

QPreferencesFilesStructuresView::QPreferencesFilesStructuresView(QWidget* pParent)
	: QWidget(pParent)
{
    QSplitter* pSplitter = new QSplitter(this);
    {
        m_pFileTable = new QListWidget(pSplitter);
        pSplitter->addWidget(m_pFileTable);
        pSplitter->setStretchFactor(0, 1);

        QWidget* pWidget = new QWidget(pSplitter);
        pSplitter->addWidget(pWidget);
        pSplitter->setStretchFactor(1, 0);

        QVBoxLayout* pVBoxLayout = new QVBoxLayout();
        pWidget->setLayout(pVBoxLayout);
        {
            pVBoxLayout->addWidget(new QWidget(), 6);
            m_pAddButton = new QPushButton(tr("&New one"));
            pVBoxLayout->addWidget(m_pAddButton, 1);
            m_pRemoveButton = new QPushButton(tr("&Remove"));
            pVBoxLayout->addWidget(m_pRemoveButton, 1);
        }
    }
}

QPreferencesFilesStructuresView::~QPreferencesFilesStructuresView()
{

}

QListWidget* QPreferencesFilesStructuresView::getTableWidget() const
{
    return m_pFileTable;
}

QPushButton* QPreferencesFilesStructuresView::getAddButton() const
{
    return m_pAddButton;
}

QPushButton* QPreferencesFilesStructuresView::getRemoveButton() const
{
    return m_pRemoveButton;
}