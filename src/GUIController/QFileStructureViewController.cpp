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

class EntryParams
{
public:
	QString szName;
	QString szType;
	QString szSize;
	QString szOffsetStart;
	QString szValue;
};

class EntryContext
{
public:
	QList<QStandardItem*> listColumns;
};

enum COLUMN {
	ColumnName = 0,
	ColumnType,
	ColumnSize,
	ColumnOffset,
	ColumnValue,
};

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
	qint64 iOffsetStart = fileToRead.pos();
	QString szOffsetStartText = QString::number(iOffsetStart);
	qint64 iOffsetEnd;

	QString szTmp;

	EntryParams entryParams;
	entryParams.szName = pItem->m_szName;
	entryParams.szType = pItem->getTypeString();
	entryParams.szSize = szSizeText;
	entryParams.szOffsetStart = szOffsetStartText;

	EntryContext entryContext;

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

		QStandardItem* pCurrentListItem;
		qint64 iOffsetStartItem;
		QString szOffsetStartItemText;
		qint64 iOffsetEndItem;

		entryParams.szSize = "0";
		appendEntry(entryParams, pParentItem, entryContext);

		bool bStop = false;
		do{
			// Check stop condition
			if(iMaxCount == -1){
				if(iOffsetStart >= fileToRead.size()){
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

				iOffsetStartItem = fileToRead.pos();
				szOffsetStartItemText = QString::number(iOffsetStartItem);

				iCount++;

				EntryParams entryParamsItem;
				entryParamsItem.szName = szTmp;
				entryParamsItem.szType = "item";
				entryParamsItem.szSize = "0";
				entryParamsItem.szOffsetStart = szOffsetStartItemText;

				EntryContext entryContextItem;
				appendEntry(entryParamsItem, entryContext.listColumns[0], entryContextItem);

				pCurrentListItem = entryContextItem.listColumns[0];

				for(iter = pItem->m_listChildren.constBegin(); iter != pItem->m_listChildren.constEnd(); ++iter){
					bRes = processFileStructureItem((*iter), fileToRead, pCurrentListItem);
					if(!bRes){
						break;
					}
				}

				iOffsetEndItem = fileToRead.pos();
				entryContextItem.listColumns[ColumnSize]->setText(QString::number(iOffsetEndItem-iOffsetStartItem));

			}
		}while(!bStop);

		if(bRes){
			iOffsetEnd = fileToRead.pos();
			entryContext.listColumns[ColumnSize]->setText(QString::number(iOffsetEnd-iOffsetStart));
		}
	}
		break;
	case FileStructureItem::INT8:
	{
		qint8 i;
		bRes = fileToRead.read((char*)&i, sizeof(i));
		szValue = QString::number(qFromBigEndian(i));
		appendEntry(entryParams, pParentItem, entryContext);
	}
		break;
	case FileStructureItem::UINT8:
	{
		quint8 i;
		bRes = fileToRead.read((char*)&i, sizeof(i));
		szValue = QString::number(qFromBigEndian(i));
		appendEntry(entryParams, pParentItem, entryContext);
	}
		break;
	case FileStructureItem::INT16:
	{
		qint16 i;
		bRes = fileToRead.read((char*)&i, sizeof(i));
		szValue = QString::number(qFromBigEndian(i));
		appendEntry(entryParams, pParentItem, entryContext);
	}
		break;
	case FileStructureItem::UINT16:
	{
		quint16 i;
		bRes = fileToRead.read((char*)&i, sizeof(i));
		szValue = QString::number(qFromBigEndian(i));
		appendEntry(entryParams, pParentItem, entryContext);
	}
		break;
	case FileStructureItem::INT32:
	{
		qint32 i;
		bRes = fileToRead.read((char*)&i, sizeof(i));
		szValue = QString::number(qFromBigEndian(i));
		appendEntry(entryParams, pParentItem, entryContext);
	}
		break;
	case FileStructureItem::UINT32:
	{
		quint32 i;
		bRes = fileToRead.read((char*)&i, sizeof(i));
		szValue = QString::number(qFromBigEndian(i));
		appendEntry(entryParams, pParentItem, entryContext);
	}
		break;
	case FileStructureItem::INT64:
	{
		qint64 i;
		bRes = fileToRead.read((char*)&i, sizeof(i));
		szValue = QString::number(qFromBigEndian(i));
		appendEntry(entryParams, pParentItem, entryContext);
	}
	break;
	case FileStructureItem::UINT64:
	{
		quint64 i;
		bRes = fileToRead.read((char*)&i, sizeof(i));
		szValue = QString::number(qFromBigEndian(i));
		appendEntry(entryParams, pParentItem, entryContext);
	}
		break;
	default:
		break;
	}

	return bRes;
}

void QFileStructureViewController::appendEntry(const EntryParams& params, QStandardItem* pParentItem, EntryContext& context)
{
	QStandardItem* pItem;

	QList<QStandardItem*>& listColumns = context.listColumns;

	// Name
	pItem = new QStandardItem((params.szName.isEmpty() ? params.szType : params.szName));
	pItem->setEditable(false);
	listColumns.append(pItem);

	// Type
	pItem = new QStandardItem(params.szType);
	pItem->setEditable(false);
	listColumns.append(pItem);

	listColumns.append(new QStandardItem(params.szSize));
	listColumns.append(new QStandardItem(params.szOffsetStart));
	listColumns.append(new QStandardItem(params.szValue));

	if(pParentItem){
		pParentItem->appendRow(listColumns);
	}else{
		m_pModel->appendRow(listColumns);
	}
}
