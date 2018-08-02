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
	bool bRes = true;
	m_pModel->clear();

	QString szStructureFilePath = "./struct_recording.xml";

	FileStructure loadedFileStructure;

	// Load structure
	if(bRes){
		QFile fileStructure;
		fileStructure.setFileName(szStructureFilePath);
		bRes = fileStructure.open(QIODevice::ReadOnly);
		if(bRes){

			QXmlInputSource source(&fileStructure);

			QXmlSimpleReader reader;
			StructureFileParserHandler handler(&loadedFileStructure);
			reader.setContentHandler(&handler);
			reader.setErrorHandler(&handler);

			bRes = reader.parse(source);
			if(!bRes){
				qWarning("[XML] Error to parse the content of file: %s", qPrintable(szStructureFilePath));
			}

			fileStructure.close();
		}else{
			qWarning("[XML] Error to open the file: %s", qPrintable(szStructureFilePath));
		}
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
