/*
 * QFileStructureViewController.cpp
 *
 *  Created on: 31 juil. 2018
 *      Author: ebeuque
 */

#include <QPushButton>

#include "GUI/QFileStructureView.h"

#include "QFileStructureViewController.h"

QFileStructureViewController::QFileStructureViewController(QFileStructureView* pFileStructureView)
{
	m_pFileStructureView = pFileStructureView;

	connect(m_pFileStructureView->getLoadButton(), SIGNAL(clicked()), this, SLOT(loadStructure()));
}

QFileStructureViewController::~QFileStructureViewController()
{

}

void QFileStructureViewController::loadStructure()
{
	qDebug("click");
}
