/*
 * QFileStructureView.cpp
 *
 *  Created on: 31 juil. 2018
 *      Author: ebeuque
 */

#include <QVBoxLayout>
#include <QTreeView>
#include <QPushButton>
#include <QHeaderView>
#include <QComboBox>
#include <QToolButton>
#include <QProgressBar>

#include "QFileStructureView.h"

QFileStructureView::QFileStructureView(QWidget* pParent)
	: QWidget(pParent)
{
	setContentsMargins(0, 0, 0, 0);

	QVBoxLayout* pMainLayout = new QVBoxLayout();
	setLayout(pMainLayout);

	QBoxLayout* pTmpLayout;

	{
		pTmpLayout = new QHBoxLayout();
		pMainLayout->addLayout(pTmpLayout);

		m_pStructureFileComboBox = new QComboBox(this);
		m_pStructureFileComboBox->setEditable(true);
		pTmpLayout->addWidget(m_pStructureFileComboBox);

		m_pRefreshFileButton = new QToolButton(this);
		m_pRefreshFileButton->setText(tr("Refresh"));
		m_pRefreshFileButton->setWindowIcon(QIcon::fromTheme("refresh"));
		pTmpLayout->addWidget(m_pRefreshFileButton);

		m_pOpenFileButton = new QToolButton(this);
		m_pOpenFileButton->setText(tr("Open"));
		m_pOpenFileButton->setWindowIcon(QIcon::fromTheme("open"));
		pTmpLayout->addWidget(m_pOpenFileButton);

	}

	m_pTreeView = new QTreeView(this);
	pMainLayout->addWidget(m_pTreeView);

	QHeaderView* pTreeHeader;
	pTreeHeader = m_pTreeView->header();
	if(pTreeHeader) {
		pTreeHeader->setStretchLastSection(true);
	}

	m_pProgressBar = new QProgressBar(this);
	m_pProgressBar->setMaximum(100);
	m_pProgressBar->setVisible(false);
	pMainLayout->addWidget(m_pProgressBar);

	m_pLoadButton = new QPushButton(tr("Load"), this);
	pMainLayout->addWidget(m_pLoadButton);
}

QFileStructureView::~QFileStructureView()
{

}

QComboBox* QFileStructureView::getStructureFileComboBox() const
{
	return m_pStructureFileComboBox;
}

QToolButton* QFileStructureView::getRefreshFileButton() const
{
	return m_pRefreshFileButton;
}

QTreeView* QFileStructureView::getTreeview() const
{
	return m_pTreeView;
}

QPushButton* QFileStructureView::getLoadButton() const
{
	return m_pLoadButton;
}

QToolButton* QFileStructureView::getOpenButton() const
{
	return m_pOpenFileButton;
}


void QFileStructureView::setModel(QAbstractItemModel* pItemModel)
{
	m_pTreeView->setModel(pItemModel);
}

void QFileStructureView::setProgressValue(int iValue)
{
	m_pProgressBar->setValue(iValue);
}

void QFileStructureView::setSearchRunning(bool bIsRunning)
{
	m_pProgressBar->setVisible(bIsRunning);
	m_pLoadButton->setEnabled(!bIsRunning);
}
