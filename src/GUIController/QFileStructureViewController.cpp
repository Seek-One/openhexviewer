/*
 * QFileStructureViewController.cpp
 *
 *  Created on: 31 juil. 2018
 *      Author: ebeuque
 */

#include <QDebug>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_VERSION_H
#include <version.h>
#endif

#include <QFile>
#include <QDir>
#include <QtEndian>
#include <QToolButton>
#include <QPushButton>
#include <QXmlInputSource>
#include <QTreeView>
#include <QComboBox>
#include <QHeaderView>
#if QT_VERSION_MAJOR >= 5
#include <QJSEngine>
#else
#include <QScriptEngine>
#endif
#include <QRegExp>
#include <QMessageBox>

#include "GUI/QFileStructureView.h"
#include "GUIModel/QFileStructureModel.h"

#include "FileStructure/FileStructure.h"
#include "FileStructure/FileStructureItem.h"
#include "FileStructure/FileStructureComplexType.h"

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
	m_iDefaultEndianness = Endianness::Auto;

	reloadStructureFiles();

	connect(m_pFileStructureView->getRefreshFileButton(), SIGNAL(clicked()), this, SLOT(reloadStructureFiles()));

	connect(m_pFileStructureView->getLoadButton(), SIGNAL(clicked()), this, SLOT(loadStructure()));

	m_pModel = new QFileStructureModel();
	m_pFileStructureView->setModel(m_pModel);
	connect(m_pFileStructureView->getTreeview()->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex &)), this, SLOT(entrySelected(const QModelIndex&, const QModelIndex &)));

}

QFileStructureViewController::~QFileStructureViewController()
{

}


void QFileStructureViewController::reloadStructureFiles()
{
	QComboBox* pComboBox = m_pFileStructureView->getStructureFileComboBox();
	pComboBox->clear();

	// Load from debug dir
	QString szFilePath = "./data/structure_files";
	loadStructureFileList(szFilePath);

	// Load from user config dir
	QDir dir = QDir::home();
#ifdef UNIX
	dir = dir.filePath(".config");
	dir = dir.filePath(APPLICATION_PACKAGE_NAME);
	dir = dir.filePath("structure_files");
#else
	dir = dir.filePath(APPLICATION_PACKAGE_NAME);
	dir = dir.filePath("structure_files");
#endif
	loadStructureFileList(dir.path());
}

void QFileStructureViewController::loadStructureFileList(const QString& szDirPath)
{
	qDebug("[GUI] Loading structure files from: %s", qPrintable(szDirPath));

	QDir dir(szDirPath);

	QStringList listFilters;
	listFilters << "*.xml";
	QStringList entryList = dir.entryList(listFilters, QDir::Files, QDir::Name);

	QComboBox* pComboBox = m_pFileStructureView->getStructureFileComboBox();

	QStringList::const_iterator iter;
	for(iter = entryList.constBegin(); iter != entryList.constEnd(); ++iter)
	{
		QString szFilePath = dir.filePath(*iter);
		pComboBox->addItem(*iter, szFilePath);
	}
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

	QComboBox* pFileStructComboBox = m_pFileStructureView->getStructureFileComboBox();
	int iIndex = pFileStructComboBox->currentIndex();
	QString szStructureFilePath;
	if(iIndex != -1){
		szStructureFilePath = pFileStructComboBox->itemData(iIndex).toString();
	}else{
		szStructureFilePath = pFileStructComboBox->currentText();
	}

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
		m_iDefaultEndianness = loadedFileStructure.getDefaultEndianness();
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
#if QT_VERSION_MAJOR >= 5
		pTreeView->header()->setSectionResizeMode(4, QHeaderView::Fixed);
#else
		pTreeView->header()->setResizeMode(4, QHeaderView::Fixed);
#endif
		pTreeView->header()->resizeSection(4, 60);
#if QT_VERSION_MAJOR >= 5 
		pTreeView->header()->setSectionResizeMode(0, QHeaderView::Interactive);
#else
		pTreeView->header()->resizeSection(0, QHeaderView::Interactive);
#endif
		pTreeView->header()->resizeSection(0, 200);
	}


	if(!bRes){
		QMessageBox::critical(m_pFileStructureView, tr("Error"), tr("Error while creating the structure of the file"));
	}
}


bool QFileStructureViewController::readFileWithStructure(const QString& szFilePath, const FileStructure& loadedFileStructure)
{
	bool bRes = false;
	QFile fileToRead(szFilePath);

	bRes = fileToRead.open(QIODevice::ReadOnly);
	if(bRes){
		DictVariable dict;
		bRes = processFileStructureItem(loadedFileStructure.getRootItem(), fileToRead, dict, NULL);
		fileToRead.close();
	}

	return bRes;
}


bool QFileStructureViewController::processFileStructureItem(const FileStructureItemSharedPtr& pItem, QFile& fileToRead, DictVariable& dict, QStandardItem* pParentItem)
{
	bool bRes = true;

	QList<FileStructureItemSharedPtr>::const_iterator iter;
	QString szValue;

	QString szTmp;
	qint64 iSizeExpr = -1;

	// Compute element size
	QString szSizeText;
	if(pItem->m_iSize >= 0){
		szSizeText = QString::number(pItem->m_iSize);
	}else if(!pItem->m_szExpr.isEmpty()){
		bRes = prepareExpr(pItem->m_szExpr, dict, szTmp);
		iSizeExpr = evaluateIntExpr(szTmp);
		szSizeText = QString::number(iSizeExpr);
	}
	if(!bRes){
		return false;
	}

	qint64 iOffsetStart = fileToRead.pos();
	QString szOffsetStartText = QString::number(iOffsetStart);
	qint64 iOffsetEnd;

	EntryParams entryParams;
	entryParams.szName = pItem->m_szName;
	entryParams.szType = pItem->getTypeString();
	entryParams.szSize = szSizeText;
	entryParams.szOffsetStart = szOffsetStartText;

	Endianness::Mode iEndiannessMode = Endianness::Auto;
	if(pItem->m_iFlags & FileStructureItem::BigEndian){
		iEndiannessMode = Endianness::BigEndian;
	}else if(pItem->m_iFlags & FileStructureItem::LittleEndian){
		iEndiannessMode = Endianness::LittleEndian;
	}else{
		iEndiannessMode = m_iDefaultEndianness;
	}

	if(iEndiannessMode == Endianness::Auto){
#if QT_VERSION_MAJOR >= 5
		iEndiannessMode = Endianness::BigEndian;
#else
		iEndiannessMode = Endianness::LittleEndian;
#endif
	}

	EntryContext entryContext;

	switch(pItem->m_type){
	case FileStructureItem::ROOT:
	{
		for(iter = pItem->m_listChildren.constBegin(); iter != pItem->m_listChildren.constEnd(); ++iter){
			bRes = processFileStructureItem((*iter), fileToRead, dict, pParentItem);
			if(!bRes){
				break;
			}
		}
	}
		break;

	case FileStructureItem::COMPLEXTYPE:
	{
		FileStructureComplexTypeSharedPtr pComplexType = pItem->m_pComplexType;

		bRes = processFileStructureItem(pComplexType->getRootItem(), fileToRead, dict, pParentItem);


		/*
		QStandardItem* pBlockItem;

		entryParams.szSize = "0";
		appendEntry(entryParams, pParentItem, entryContext);

		pBlockItem = entryContext.listColumns[0];

		for(iter = pItem->m_listChildren.constBegin(); iter != pItem->m_listChildren.constEnd(); ++iter){
			bRes = processFileStructureItem((*iter), fileToRead, dict, pBlockItem);
			if(!bRes){
				break;
			}
		}

		if(bRes){
			iOffsetEnd = fileToRead.pos();
			entryContext.listColumns[ColumnSize]->setText(QString::number(iOffsetEnd-iOffsetStart));
		}*/
	}
	break;
	case FileStructureItem::BLOCK:
	{
		QStandardItem* pBlockItem;

		entryParams.szSize = "0";
		appendEntry(entryParams, pParentItem, entryContext);

		pBlockItem = entryContext.listColumns[0];

		for(iter = pItem->m_listChildren.constBegin(); iter != pItem->m_listChildren.constEnd(); ++iter){
			bRes = processFileStructureItem((*iter), fileToRead, dict, pBlockItem);
			if(!bRes){
				break;
			}
		}

		if(bRes){
			iOffsetEnd = fileToRead.pos();
			entryContext.listColumns[ColumnSize]->setText(QString::number(iOffsetEnd-iOffsetStart));
		}
	}
		break;
	case FileStructureItem::LIST:
	{
		qint64 iMaxSize = iSizeExpr;
		qint64 iCount = 0;
		qint64 iOffsetStartList = fileToRead.pos();


		QStandardItem* pCurrentListItem;
		qint64 iOffsetStartItem;
		QString szOffsetStartItemText;
		qint64 iOffsetEndItem;
		qint64 iOffsetCurrent;

		bool bIsFlat = (pItem->m_iFlags & FileStructureItem::FlatList);

		entryParams.szSize = "0";
		if(!bIsFlat){
			appendEntry(entryParams, pParentItem, entryContext);
		}

		FileStructureItem::SizeMode iSizeMode = pItem->m_iSizeMode;

		bool bStop = false;
		do{
			// Check stop condition
			iOffsetCurrent = fileToRead.pos();
			if(iMaxSize == -1){
				if(iOffsetCurrent >= fileToRead.size()){
					bStop = true;
				}
			}else{
				if(iSizeMode == FileStructureItem::ModeCount){
					if(iCount == iMaxSize){
						// If count is set to
						bStop = true;
					}
				}else{
					if((iOffsetCurrent - iOffsetStartList) >= iMaxSize){
						bStop = true;
					}
				}
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
				if(bIsFlat){
					appendEntry(entryParamsItem, pParentItem, entryContextItem);
				}else{
					appendEntry(entryParamsItem, entryContext.listColumns[0], entryContextItem);
				}

				pCurrentListItem = entryContextItem.listColumns[0];

				for(iter = pItem->m_listChildren.constBegin(); iter != pItem->m_listChildren.constEnd(); ++iter){
					bRes = processFileStructureItem((*iter), fileToRead, dict, pCurrentListItem);
					if(!bRes){
						break;
					}
				}

				iOffsetEndItem = fileToRead.pos();
				entryContextItem.listColumns[ColumnSize]->setText(QString::number(iOffsetEndItem-iOffsetStartItem));

				if(!m_szListItemName.isNull()){
					entryContextItem.listColumns[0]->setText(m_szListItemName);
					m_szListItemName = QString();
				}

			}
		}while(!bStop && bRes);

		if(bRes){
			iOffsetEnd = fileToRead.pos();
			if(!bIsFlat){
				entryContext.listColumns[ColumnSize]->setText(QString::number(iOffsetEnd-iOffsetStart));
			}
		}
	}
		break;
	case FileStructureItem::LIST_ITEM_INFOS:
	{
		QString szExpr;
		bRes = prepareExpr(pItem->m_szExpr, dict, szExpr);
		m_szListItemName = szExpr;
		break;
	}
	case FileStructureItem::COND:
	{
		QString szExpr;
		bRes = prepareExpr(pItem->m_szExpr, dict, szExpr);

		bool bExprResult = false;
		if(bRes){
			bRes = evaluateBooleanExpr(szExpr, bExprResult);
		}
		//qDebug() << pItem->m_szExpr << " = " << szExpr << " : " << bExprResult;

		if(bRes && bExprResult){
			for(iter = pItem->m_listChildren.constBegin(); iter != pItem->m_listChildren.constEnd(); ++iter){
				bRes = processFileStructureItem((*iter), fileToRead, dict, pParentItem);
				if(!bRes){
					break;
				}
			}
		}
	}
		break;
	case FileStructureItem::INT8:
	{
		qint8 i;
		bRes = fileToRead.read((char*)&i, sizeof(i));
		switch(iEndiannessMode){
		case Endianness::BigEndian:
#if QT_VERSION_MAJOR >= 5
			entryParams.szValue = QString::number(qFromBigEndian<qint8>(i));
#else
			entryParams.szValue = QString::number(i);
#endif
			break;
		default:
			entryParams.szValue = QString::number(i); break;
		}
		appendDict(dict, entryParams.szName, entryParams.szValue);
		appendEntry(entryParams, pParentItem, entryContext);
	}
		break;
	case FileStructureItem::UINT8:
	{
		quint8 i;
		bRes = fileToRead.read((char*)&i, sizeof(i));
		switch(iEndiannessMode){
		case Endianness::BigEndian:
#if QT_VERSION_MAJOR >= 5
			entryParams.szValue = QString::number(qFromBigEndian<quint8>(i));
#else
			entryParams.szValue = QString::number(i);
#endif
			break;
		default:
			entryParams.szValue = QString::number(i); break;
		}
		appendDict(dict, entryParams.szName, entryParams.szValue);
		appendEntry(entryParams, pParentItem, entryContext);
	}
		break;
	case FileStructureItem::INT16:
	{
		qint16 i;
		bRes = fileToRead.read((char*)&i, sizeof(i));
		switch(iEndiannessMode){
		case Endianness::BigEndian:
			entryParams.szValue = QString::number(qFromBigEndian<qint16>(i));
			break;
		default:
			entryParams.szValue = QString::number(i); break;
		}
		appendDict(dict, entryParams.szName, entryParams.szValue);
		appendEntry(entryParams, pParentItem, entryContext);
	}
		break;
	case FileStructureItem::UINT16:
	{
		quint16 i;
		bRes = fileToRead.read((char*)&i, sizeof(i));
		switch(iEndiannessMode){
		case Endianness::BigEndian:
#if QT_VERSION_MAJOR >= 5
			entryParams.szValue = QString::number(qFromBigEndian<quint16>(i));
#else
			entryParams.szValue = QString::number((quint16)qFromBigEndian<qint16>((qint16)i));
#endif
			break;
		default:
			entryParams.szValue = QString::number(i); break;
		}
		appendDict(dict, entryParams.szName, entryParams.szValue);
		appendEntry(entryParams, pParentItem, entryContext);
	}
		break;
	case FileStructureItem::INT32:
	{
		qint32 i;
		bRes = fileToRead.read((char*)&i, sizeof(i));
		switch(iEndiannessMode){
		case Endianness::BigEndian:
			entryParams.szValue = QString::number(qFromBigEndian<qint32>(i));
			break;
		default:
			entryParams.szValue = QString::number(i); break;
		}
		appendDict(dict, entryParams.szName, entryParams.szValue);
		appendEntry(entryParams, pParentItem, entryContext);
	}
		break;
	case FileStructureItem::UINT32:
	{
		quint32 i;
		bRes = fileToRead.read((char*)&i, sizeof(i));
		switch(iEndiannessMode){
		case Endianness::BigEndian:
#if QT_VERSION_MAJOR >= 5
			entryParams.szValue = QString::number(qFromBigEndian<quint32>(i));
#else
			entryParams.szValue = QString::number((quint32)qFromBigEndian<qint32>((qint32)i));
#endif
			break;
		default:
			entryParams.szValue = QString::number(i); break;
		}
		appendDict(dict, entryParams.szName, entryParams.szValue);
		appendEntry(entryParams, pParentItem, entryContext);
	}
		break;
	case FileStructureItem::INT64:
	{
		qint64 i;
		bRes = fileToRead.read((char*)&i, sizeof(i));
		switch(iEndiannessMode){
		case Endianness::BigEndian:
			entryParams.szValue = QString::number(qFromBigEndian<qint64>(i));
			break;
		default:
			entryParams.szValue = QString::number(i); break;
		}
		appendDict(dict, entryParams.szName, entryParams.szValue);
		appendEntry(entryParams, pParentItem, entryContext);
	}
	break;
	case FileStructureItem::UINT64:
	{
		quint64 i;
		bRes = fileToRead.read((char*)&i, sizeof(i));
		switch(iEndiannessMode){
		case Endianness::BigEndian:
#if QT_VERSION_MAJOR >= 5
			entryParams.szValue = QString::number(qFromBigEndian<quint64>(i));
#else
			entryParams.szValue = QString::number((quint64)qFromBigEndian<qint64>((qint64)i));
#endif
			break;
		default:
			entryParams.szValue = QString::number(i); break;
		}
		appendDict(dict, entryParams.szName, entryParams.szValue);
		appendEntry(entryParams, pParentItem, entryContext);
	}
		break;
	case FileStructureItem::BYTES:
	{
		if(iSizeExpr > 0){
			bRes = fileToRead.seek(iOffsetStart + iSizeExpr);
			appendEntry(entryParams, pParentItem, entryContext);
		}
	}
		break;
	case FileStructureItem::STRING:
	{
        if(iSizeExpr > 0){
		    char* szString = new char[iSizeExpr+1];
		    bRes = fileToRead.read((char*)szString, iSizeExpr);
		    szString[iSizeExpr] = '\0';
		    entryParams.szValue = szString;
        }else{
		    entryParams.szValue = "";
        }
		appendDict(dict, entryParams.szName, entryParams.szValue);
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

void QFileStructureViewController::appendDict(DictVariable& dict, const QString& szName, const QString& szValue)
{
	dict.insert(szName, szValue);
}

void QFileStructureViewController::entrySelected(const QModelIndex &current, const QModelIndex &previous)
{
	QModelIndex siblingIndex;
	QStandardItem* pItem;

	qint64 iOffset;
	qint64 iSize;

	siblingIndex = m_pModel->sibling(current.row(), ColumnSize, current);
	pItem = m_pModel->itemFromIndex(siblingIndex);
	iSize = pItem->text().toLongLong();

	siblingIndex = m_pModel->sibling(current.row(), ColumnOffset, current);
	pItem = m_pModel->itemFromIndex(siblingIndex);
	iOffset = pItem->text().toLongLong();

	emit fileStructureItemSelected(iOffset, iSize);
}

bool QFileStructureViewController::prepareExpr(const QString& szExpression, const DictVariable& dict, QString& szNewExpression)
{
	bool bRes = true;

	/*
	qDebug() << "dict";
	DictVariable::const_iterator iter;
	for(iter = dict.constBegin(); iter != dict.constEnd(); ++iter)
	{
		qDebug() << iter.key() << "=" << iter.value();
	}
	*/

	szNewExpression = szExpression;

	QRegExp rx("\\$\\{([A-Za-z0-9]+)\\}*");

	int iCurrentPos = 0;
	int iFoundPos;

	QString szVarName;
	QString szDictValue;

	do {
		iFoundPos = rx.indexIn(szExpression, iCurrentPos);
		if(iFoundPos != -1){
			iCurrentPos += iFoundPos;

			QStringList tokens = rx.capturedTexts();
			if(tokens.count() >= 2){
				szVarName = tokens[1];
				iCurrentPos += (szVarName.size() + 3);

				if(dict.contains(szVarName)){
					szDictValue = dict.value(szVarName);
					szNewExpression = szNewExpression.replace("${" + szVarName + "}", szDictValue);
				}
			}
		}
	}while(iFoundPos != -1);

	szNewExpression = szNewExpression.replace("AND", "&&");
	szNewExpression = szNewExpression.replace("OR", "||");

	return bRes;
}

bool QFileStructureViewController::evaluateBooleanExpr(const QString& szExpression, bool& bResult)
{
#if QT_VERSION_MAJOR >= 5
	QJSEngine expression;
	QJSValue result = expression.evaluate(szExpression);
	if(result.isBool()){
		bResult = result.toBool();
		return true;
	}else if(result.isError()){
		qWarning("[FileStructure] Error to evaluate boolean expression : %s", qPrintable(szExpression));
	}
	return false;
#else
	QScriptEngine expression;
	bResult = expression.evaluate(szExpression).toBool();
	return true;
#endif
}

int QFileStructureViewController::evaluateIntExpr(const QString& szExpression)
{
#if QT_VERSION_MAJOR >= 5
	QJSEngine expression;
	int iRes = expression.evaluate(szExpression).toInt();
#else
	QScriptEngine expression;
	int iRes = (int)expression.evaluate(szExpression).toInt32();
#endif
	return iRes;
}

bool QFileStructureViewController::isNumber(const QString& szStr) const
{
	for (int i =0;i<szStr.size();i++)
	{
		if (!szStr[i].isDigit()){
			return false;
		}
	}
	return true;
}
