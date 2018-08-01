/*
 * QFileStructureViewController.cpp
 *
 *  Created on: 31 juil. 2018
 *      Author: ebeuque
 */

#include <QFile>
#include <QPushButton>
#include <QXmlInputSource>

#include "GUI/QFileStructureView.h"
#include "GUIModel/QFileStructureModel.h"

#include "XML/StructureFileParserHandler.h"

#include "QFileStructureViewController.h"

QFileStructureViewController::QFileStructureViewController(QFileStructureView* pFileStructureView)
{
	m_pFileStructureView = pFileStructureView;

	connect(m_pFileStructureView->getLoadButton(), SIGNAL(clicked()), this, SLOT(loadStructure()));

	m_pModel = new QFileStructureModel();
	m_pFileStructureView->setModel(m_pModel);
}

QFileStructureViewController::~QFileStructureViewController()
{

}

void QFileStructureViewController::setCurrentFile(const QString& szFilePath)
{
	m_szCurrentFilePath = szFilePath;
}

void QFileStructureViewController::loadStructure()
{
	m_pModel->clear();

	QFile fileStructure;
	fileStructure.setFileName("./struct_recording.xml");

	if(fileStructure.open(QIODevice::ReadOnly)){

		QXmlInputSource source(&fileStructure);

		QXmlSimpleReader reader;
		StructureFileParserHandler handler(m_szCurrentFilePath, m_pModel);
		reader.setContentHandler(&handler);
		reader.setErrorHandler(&handler);

		if(!reader.parse(source)){
			qWarning("[XML] Error to parse the content of file: %s", qPrintable(m_szCurrentFilePath));
		}

		fileStructure.close();
	}

/*

	for(int i=0; i<10; i++){
		QStandardItem* pItem = new QStandardItem("test");

		QList<QStandardItem*> listColumn;
		listColumn.append(new QStandardItem("test2"));
		listColumn.append(new QStandardItem("test3"));
		pItem->appendRow(listColumn);

		m_pModel->appendRow(pItem);
	}
*/
}
