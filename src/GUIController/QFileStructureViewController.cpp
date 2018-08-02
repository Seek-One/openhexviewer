/*
 * QFileStructureViewController.cpp
 *
 *  Created on: 31 juil. 2018
 *      Author: ebeuque
 */

#include <QFile>
#include <QtEndian>
#include <QPushButton>
#include <QXmlInputSource>
#include <QTreeView>
#include <QHeaderView>

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

	// Reinitialize item
	m_pModel->clear();
	m_pModel->setHorizontalHeaderItem(0, new QStandardItem(tr("Name")));
	m_pModel->setHorizontalHeaderItem(1, new QStandardItem(tr("Type")));
	m_pModel->setHorizontalHeaderItem(2, new QStandardItem(tr("Size")));
	m_pModel->setHorizontalHeaderItem(3, new QStandardItem(tr("Offset")));
	m_pModel->setHorizontalHeaderItem(4, new QStandardItem(tr("Value")));

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
				qCritical("[XML] Error to parse the content of file: %s", qPrintable(szStructureFilePath));
			}

			fileStructure.close();
		}else{
			qCritical("[XML] Error to open the file: %s", qPrintable(szStructureFilePath));
		}
	}

	if(bRes){
		bRes = readFileWithStructure(m_szCurrentFilePath, loadedFileStructure);
		if(!bRes){
			qCritical("[FileStructure] Cannot read file with structure");
		}
	}

	if(bRes){
		QTreeView* pTreeView = m_pFileStructureView->getTreeview();
		pTreeView->resizeColumnToContents(1);
		pTreeView->resizeColumnToContents(2);
		pTreeView->resizeColumnToContents(3);
		pTreeView->header()->setSectionResizeMode(4, QHeaderView::Fixed);
		pTreeView->header()->resizeSection(4, 60);
		pTreeView->header()->setSectionResizeMode(0, QHeaderView::Interactive);
		pTreeView->header()->resizeSection(0, 200);
	}
}


bool QFileStructureViewController::readFileWithStructure(const QString& szFilePath, const FileStructure& loadedFileStructure)
{
	bool bRes = false;
	QFile fileToRead(szFilePath);

	bRes = fileToRead.open(QIODevice::ReadOnly);
	if(bRes){
		processFileStructureItem(loadedFileStructure.getRootItem(), fileToRead, NULL);
		fileToRead.close();
	}

	return bRes;
}


bool QFileStructureViewController::processFileStructureItem(const FileStructureItemSharedPtr& pItem, QFile& fileToRead, QStandardItem* pParentItem)
{
	bool bRes = true;

	QList<FileStructureItemSharedPtr>::const_iterator iter;
	QString szValue;

	QString szSizeText = QString::number(pItem->m_iSize);
	qint64 iOffset = fileToRead.pos();
	QString szOffsetText = QString::number(iOffset);

	QStandardItem* pCurrentItem;
	QString szTmp;

	switch(pItem->m_type){
	case FileStructureItem::ROOT:
	{
		for(iter = pItem->m_listChildren.constBegin(); iter != pItem->m_listChildren.constEnd(); ++iter){
			bRes = processFileStructureItem((*iter), fileToRead, pParentItem);
			if(!bRes){
				break;
			}
		}
	}
		break;
	case FileStructureItem::LIST:
	{
		qint64 iMaxCount = pItem->m_iSize;
		qint64 iCount = 0;

		QStandardItem* pListItem = appendEntry(pItem->m_szName, "list", "0", szOffsetText, "", pParentItem);

		bool bStop = false;
		do{
			// Check stop condition
			if(iMaxCount == -1){
				if(iOffset >= fileToRead.size()){
					bStop = true;
				}
			}else if(iCount == iMaxCount){
				// If count is set to
				bStop = true;
			}

			// Iterate over all children
			if(!bStop){

				if(pItem->m_szName.isEmpty()){
					szTmp = QString("item[%0]").arg(iCount);
				}else{
					szTmp = QString("%0[%1]").arg(pItem->m_szName).arg(iCount);
				}

				iCount++;

				pCurrentItem = appendEntry(szTmp, "struct", "0", szOffsetText, "", pListItem);

				for(iter = pItem->m_listChildren.constBegin(); iter != pItem->m_listChildren.constEnd(); ++iter){
					bRes = processFileStructureItem((*iter), fileToRead, pCurrentItem);
					if(!bRes){
						break;
					}
				}
			}
		}while(!bStop);

		if(bRes){
			//pListItem-> ;
		}
	}
		break;
	case FileStructureItem::INT8:
	{
		qint8 i;
		bRes = fileToRead.read((char*)&i, sizeof(i));
		szValue = QString::number(qFromBigEndian(i));
		appendEntry(pItem->m_szName, "int8", szSizeText, szOffsetText, szValue, pParentItem);
	}
		break;
	case FileStructureItem::UINT8:
	{
		quint8 i;
		bRes = fileToRead.read((char*)&i, sizeof(i));
		szValue = QString::number(qFromBigEndian(i));
		appendEntry(pItem->m_szName, "uint8", szSizeText, szOffsetText, szValue, pParentItem);
	}
		break;
	case FileStructureItem::INT16:
	{
		qint16 i;
		bRes = fileToRead.read((char*)&i, sizeof(i));
		szValue = QString::number(qFromBigEndian(i));
		appendEntry(pItem->m_szName, "int16", szSizeText, szOffsetText, szValue, pParentItem);
	}
		break;
	case FileStructureItem::UINT16:
	{
		quint16 i;
		bRes = fileToRead.read((char*)&i, sizeof(i));
		szValue = QString::number(qFromBigEndian(i));
		appendEntry(pItem->m_szName, "uint16", szSizeText, szOffsetText, szValue, pParentItem);
	}
		break;
	case FileStructureItem::INT32:
	{
		qint32 i;
		bRes = fileToRead.read((char*)&i, sizeof(i));
		szValue = QString::number(qFromBigEndian(i));
		appendEntry(pItem->m_szName, "int32", szSizeText, szOffsetText, szValue, pParentItem);
	}
		break;
	case FileStructureItem::UINT32:
	{
		quint32 i;
		bRes = fileToRead.read((char*)&i, sizeof(i));
		szValue = QString::number(qFromBigEndian(i));
		appendEntry(pItem->m_szName, "uint32", szSizeText, szOffsetText, szValue, pParentItem);
	}
		break;
	case FileStructureItem::INT64:
	{
		qint64 i;
		bRes = fileToRead.read((char*)&i, sizeof(i));
		szValue = QString::number(qFromBigEndian(i));
		appendEntry(pItem->m_szName, "int64", szSizeText, szOffsetText, szValue, pParentItem);
	}
	break;
	case FileStructureItem::UINT64:
	{
		quint64 i;
		bRes = fileToRead.read((char*)&i, sizeof(i));
		szValue = QString::number(qFromBigEndian(i));
		appendEntry(pItem->m_szName, "uint64", szSizeText, szOffsetText, szValue, pParentItem);
	}
		break;
	default:
		break;
	}

	return bRes;
}

QStandardItem* QFileStructureViewController::appendEntry(const QString& szName, const QString& szType, const QString& szSize, const QString& szOffsetStart, const QString& szValue, QStandardItem* pParentItem)
{
	QStandardItem* pMainColumn;

	QList<QStandardItem*> listColumns;
	pMainColumn = new QStandardItem((szName.isEmpty() ? szType : szName));
	pMainColumn->setEditable(false);
	listColumns.append(pMainColumn);
	listColumns.append(new QStandardItem(szType));
	listColumns.append(new QStandardItem(szSize));
	listColumns.append(new QStandardItem(szOffsetStart));
	listColumns.append(new QStandardItem(szValue));

	if(pParentItem){
		pParentItem->appendRow(listColumns);
	}else{
		m_pModel->appendRow(listColumns);
	}

	return pMainColumn;
}
