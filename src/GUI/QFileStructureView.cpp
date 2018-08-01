/*
 * QFileStructureView.cpp
 *
 *  Created on: 31 juil. 2018
 *      Author: ebeuque
 */

#include <QVBoxLayout>
#include <QTreeView>
#include <QPushButton>

#include "QFileStructureView.h"

QFileStructureView::QFileStructureView(QWidget* pParent)
	: QWidget(pParent)
{
	setContentsMargins(0, 0, 0, 0);

	QVBoxLayout* pMainLayout = new QVBoxLayout();
	setLayout(pMainLayout);

	m_pTreeView = new QTreeView(this);
	pMainLayout->addWidget(m_pTreeView);

	m_pLoadButton = new QPushButton(tr("Load"), this);
	pMainLayout->addWidget(m_pLoadButton);
}

QFileStructureView::~QFileStructureView()
{

}

QPushButton* QFileStructureView::getLoadButton() const
{
	return m_pLoadButton;
}

void QFileStructureView::setModel(QAbstractItemModel* pItemModel)
{
	m_pTreeView->setModel(pItemModel);
}
