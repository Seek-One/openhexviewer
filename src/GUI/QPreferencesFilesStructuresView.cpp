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
        connect(m_pFileTable, &QListWidget::itemClicked, [this](QListWidgetItem* pItem) {
            emit listWidgetItemClicked(pItem);
        });
        pSplitter->addWidget(m_pFileTable);
        pSplitter->setStretchFactor(0, 1);

        QWidget* pWidget = new QWidget(pSplitter);
        pSplitter->addWidget(pWidget);
        pSplitter->setStretchFactor(1, 0);

        QVBoxLayout* pVBoxLayout = new QVBoxLayout();
        pWidget->setLayout(pVBoxLayout);
        {
            pVBoxLayout->addWidget(new QWidget(), 6);
            QPushButton* pAddButton = new QPushButton(tr("&New one"));
            connect(pAddButton, &QAbstractButton::clicked, [this]() {
                emit addButtonClicked();
            });
            pVBoxLayout->addWidget(pAddButton, 1);
            QPushButton* pRemoveButton = new QPushButton(tr("&Remove"));
            connect(pRemoveButton, &QAbstractButton::clicked, [this]() {
                emit removeButtonClicked();
            });
            pVBoxLayout->addWidget(pRemoveButton, 1);
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