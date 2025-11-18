/*
 * QFileStructureViewController.cpp
 *
 *  Created on: 31 juil. 2018
 *      Author: ebeuque
 */

#include <QDebug>
#include <QCoreApplication>

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
#include <QXmlStreamReader>
#include <QTreeView>
#include <QComboBox>
#include <QHeaderView>
#if QT_VERSION_MAJOR >= 5
#include <QJSEngine>
#else
#include <QScriptEngine>
#endif

#include <QMessageBox>
#include <QFileDialog>

#include "Global/QtCompat.h"

#ifdef USE_NO_QREGEXP
#include <QRegularExpression>
#else
#include <QRegExp>
#endif

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
	m_iTraceLevel = 0;

	reloadStructureFiles();

	connect(m_pFileStructureView->getRefreshFileButton(), SIGNAL(clicked()), this, SLOT(reloadStructureFiles()));

	connect(m_pFileStructureView->getLoadButton(), SIGNAL(clicked()), this, SLOT(loadStructure()));

	connect(m_pFileStructureView->getOpenButton(), SIGNAL(clicked()), this, SLOT(openStructureFile()));

	m_pModel = new QFileStructureModel();
	m_pFileStructureView->setModel(m_pModel);
	connect(m_pFileStructureView->getTreeview()->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex &)), this, SLOT(entrySelected(const QModelIndex&, const QModelIndex &)));

}

QFileStructureViewController::~QFileStructureViewController()
{

}

void QFileStructureViewController::reload()
{
	reloadStructureFiles();
}


void QFileStructureViewController::reloadStructureFiles()
{
	QComboBox* pComboBox = m_pFileStructureView->getStructureFileComboBox();
	pComboBox->clear();

	// Load from application directory
	QDir dir;
#if defined(WIN32)
	dir.setPath(QCoreApplication::applicationDirPath() + "/structure_files");
#elif defined(APPLE)
	dir.setPath(QCoreApplication::applicationDirPath() + "/../Resources/structure_files");
#else
	dir.setPath(QCoreApplication::applicationDirPath() + "/../share/" + APPLICATION_PACKAGE_NAME + "/structure_files");
#endif
	loadStructureFileList(tr("System"), dir.path());

	// Load from debug dir
	QString szFilePath = "./data/structure_files";
	loadStructureFileList(tr("Development"), szFilePath);

	// Load from user config dir
	dir = QDir::home();
#ifdef UNIX
	dir.setPath(dir.filePath(".config"));
	dir.setPath(dir.filePath(APPLICATION_PACKAGE_NAME));
	dir.setPath(dir.filePath("structure_files"));
#else
	dir.setPath(dir.filePath(APPLICATION_PACKAGE_NAME));
	dir.setPath(dir.filePath("structure_files"));
#endif
	loadStructureFileList(tr("User"), dir.path());
}

void QFileStructureViewController::loadStructureFileList(const QString& szOrigin, const QString& szDirPath)
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
		QString szText = *iter + " (" + szOrigin + ")";
		QString szFilePath = dir.filePath(*iter);
		pComboBox->addItem(szText, szFilePath);
	}
}

void QFileStructureViewController::openStructureFile()
{
	QFileDialog dialog(m_pFileStructureView);
	dialog.setFileMode(QFileDialog::AnyFile);
	dialog.setViewMode(QFileDialog::Detail);
	dialog.setNameFilter(tr("XML files (*.xml)"));

	if (dialog.exec()){
		QStringList listSelectedFiles;
		listSelectedFiles = dialog.selectedFiles();
		
		QComboBox* pComboBox = m_pFileStructureView->getStructureFileComboBox();
	    
		QFileInfo tFileInfo(listSelectedFiles[0]);
		pComboBox->addItem(tFileInfo.fileName() + tr(" (Open by user)") , listSelectedFiles[0]);

		pComboBox->setCurrentIndex(pComboBox->count() - listSelectedFiles.size());
		loadStructure();
	}
}

void QFileStructureViewController::setCurrentFile(const QString& szFilePath)
{
	m_szCurrentFilePath = szFilePath;
}

void QFileStructureViewController::loadStructure()
{
	bool bRes = true;

	m_iTraceLevel = 0;

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
			QXmlStreamReader source(&fileStructure);

			QXmlStreamReader reader;
			reader.addData(fileStructure.readAll());
			StructureFileParserHandler handler(&loadedFileStructure);
			bRes = handler.parse(reader);
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

	m_pFileStructureView->setSearchRunning(false);

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
		m_iFileSize = fileToRead.size();
		m_pFileStructureView->setSearchRunning(true);
		m_pFileStructureView->setProgressValue(0);

		DictVariable dict;
		bRes = processFileStructureItem(loadedFileStructure.getRootItem(), fileToRead, dict, NULL, true);
		fileToRead.close();
	}

	return bRes;
}


bool QFileStructureViewController::processFileStructureItem(const FileStructureItemSharedPtr& pItem, QFile& fileToRead, DictVariable& dict, QStandardItem* pParentItem, bool bParentVisible)
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

	bool bIsVisible = bParentVisible &&  !(pItem->m_iFlags & FileStructureItem::DisplayNone);
	bool bIsFlat = (pItem->m_iFlags & FileStructureItem::DisplayFlat);

	traceBegin(pItem->m_type, pItem->m_szName, fileToRead);

	EntryContext entryContext;

	switch(pItem->m_type){
	case FileStructureItem::ROOT:
	{
		traceInfos(pItem->m_type, pItem->m_szName, QString("childrens:%0").arg(pItem->m_listChildren.count()));
		for(iter = pItem->m_listChildren.constBegin(); iter != pItem->m_listChildren.constEnd(); ++iter){
			bRes = processFileStructureItem((*iter), fileToRead, dict, pParentItem, bIsVisible);
			if(!bRes){
				break;
			}
		}
	}
		break;

	case FileStructureItem::FIELDCOMPLEXTYPE:
	{
		FileStructureComplexTypeSharedPtr pComplexType = pItem->m_pComplexType;

		QStandardItem* pComplexTypeItem = NULL;
		entryParams.szSize = "0";
		entryParams.szType = pComplexType->getName();
		if(bIsVisible){
			if(bIsFlat){
				pComplexTypeItem = pParentItem;
			}else{
				appendEntry(entryParams, pParentItem, entryContext);
				pComplexTypeItem = entryContext.listColumns[0];
			}
		}else{
			pComplexTypeItem = pParentItem;
        }

		traceInfos(pItem->m_type, pItem->m_szName, QString("type:%0").arg(pComplexType->getName()));


		bRes = processFileStructureItem(pComplexType->getRootItem(), fileToRead, dict, pComplexTypeItem, bIsVisible);

		if(bRes && bIsVisible){
			iOffsetEnd = fileToRead.pos();
			if(!bIsFlat){
				entryContext.listColumns[ColumnSize]->setText(QString::number(iOffsetEnd-iOffsetStart));
			}
		}
	}
	break;

	case FileStructureItem::COMPLEXTYPE:
	{
		traceInfos(pItem->m_type, pItem->m_szName, QString("childrens:%0").arg(pItem->m_listChildren.count()));
		for(iter = pItem->m_listChildren.constBegin(); iter != pItem->m_listChildren.constEnd(); ++iter){
			bRes = processFileStructureItem((*iter), fileToRead, dict, pParentItem, bIsVisible);
			if(!bRes){
				break;
			}
		}
	}
	break;

	case FileStructureItem::SEEK:
	{
		traceInfos(pItem->m_type, pItem->m_szName, QString("mode:%0, offset:%1").arg(pItem->m_iSeekMode).arg(iSizeExpr));
		switch(pItem->m_iSeekMode){
		case FileStructureItem::SeekModeAbsolute:
			fileToRead.seek(iSizeExpr); break;
		case FileStructureItem::SeekModeBackward:
			fileToRead.seek(fileToRead.pos() - iSizeExpr); break;
		case FileStructureItem::SeekModeForward:
			fileToRead.seek(fileToRead.pos() + iSizeExpr); break;
		default:
			break;
		}

	}
	break;

	case FileStructureItem::BLOCK:
	{
		QStandardItem* pBlockItem;

		entryParams.szSize = "0";
		if(bIsVisible){
			appendEntry(entryParams, pParentItem, entryContext);
		}

		pBlockItem = entryContext.listColumns[0];

		for(iter = pItem->m_listChildren.constBegin(); iter != pItem->m_listChildren.constEnd(); ++iter){
			bRes = processFileStructureItem((*iter), fileToRead, dict, pBlockItem, bIsVisible);
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

		traceInfos(pItem->m_type, pItem->m_szName, QString("mode:%0, maxsize:%1, childrens:%2").arg(pItem->m_iSizeMode).arg(iMaxSize).arg(pItem->m_listChildren.count()));

		entryParams.szSize = "0";
		if(bIsVisible && !bIsFlat){
			appendEntry(entryParams, pParentItem, entryContext);
		}

		FileStructureItem::SizeMode iSizeMode = pItem->m_iSizeMode;

		DictVariable dictItem = dict;

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
				if(bIsVisible){
					if(bIsFlat){
						appendEntry(entryParamsItem, pParentItem, entryContextItem);
					}else{
						appendEntry(entryParamsItem, entryContext.listColumns[0], entryContextItem);
					}
				}

				pCurrentListItem = entryContextItem.listColumns[0];

				m_stackListItemInfos.push(QString());

				for(iter = pItem->m_listChildren.constBegin(); iter != pItem->m_listChildren.constEnd(); ++iter){
					bRes = processFileStructureItem((*iter), fileToRead, dictItem, pCurrentListItem, bIsVisible);
					if(!bRes){
						break;
					}
				}

				iOffsetEndItem = fileToRead.pos();
				entryContextItem.listColumns[ColumnSize]->setText(QString::number(iOffsetEndItem-iOffsetStartItem));

				// Handle list infos
				QString szListItemName;
				szListItemName = m_stackListItemInfos.top();
				if(!szListItemName.isNull()){
					entryContextItem.listColumns[0]->setText(szListItemName);
				}
				m_stackListItemInfos.pop();

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
		if(!m_stackListItemInfos.isEmpty()){
			m_stackListItemInfos.top() = szExpr;
		}
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
		traceInfos(pItem->m_type, pItem->m_szName, QString("cond:%0, condexpr:%1, result:%2").arg(pItem->m_szExpr).arg(szExpr).arg(bExprResult));

		if(bRes && bExprResult){
			for(iter = pItem->m_listChildren.constBegin(); iter != pItem->m_listChildren.constEnd(); ++iter){
				bRes = processFileStructureItem((*iter), fileToRead, dict, pParentItem, bIsVisible);
				if(!bRes){
					break;
				}
			}
		}
	}
		break;
	case FileStructureItem::INT8:
	{
		if (pItem->m_szValue.isEmpty()) {
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
					entryParams.szValue = QString::number(i);
					break;
			}
		}else {
			entryParams.szValue = pItem->m_szValue;
		}
		appendDict(dict, entryParams.szName, entryParams.szValue);
		if(bIsVisible){
			appendEntry(entryParams, pParentItem, entryContext);
		}
	}
		break;
	case FileStructureItem::UINT8:
	{
		if (pItem->m_szValue.isEmpty()) {
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
				entryParams.szValue = QString::number(i);
				break;
			}
		}else {
			entryParams.szValue = pItem->m_szValue;
		}
		appendDict(dict, entryParams.szName, entryParams.szValue);
		if(bIsVisible){
			appendEntry(entryParams, pParentItem, entryContext);
		}
	}
		break;
	case FileStructureItem::INT16:
	{
		if (pItem->m_szValue.isEmpty()) {
			qint16 i;
			bRes = fileToRead.read((char*)&i, sizeof(i));
			switch(iEndiannessMode){
			case Endianness::BigEndian:
				entryParams.szValue = QString::number(qFromBigEndian<qint16>(i));
				break;
			default:
				entryParams.szValue = QString::number(i);
				break;
			}
		}else {
			entryParams.szValue = pItem->m_szValue;
		}
		appendDict(dict, entryParams.szName, entryParams.szValue);
		if(bIsVisible){
			appendEntry(entryParams, pParentItem, entryContext);
		}
	}
		break;
	case FileStructureItem::UINT16:
	{
		if (pItem->m_szValue.isEmpty()) {
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
				entryParams.szValue = QString::number(i);
				break;
			}
		}else {
			entryParams.szValue = pItem->m_szValue;
		}
		appendDict(dict, entryParams.szName, entryParams.szValue);
		if(bIsVisible){
			appendEntry(entryParams, pParentItem, entryContext);
		}
	}
		break;
	case FileStructureItem::INT32:
	{
		if (pItem->m_szValue.isEmpty()) {
			qint32 i;
			bRes = fileToRead.read((char*)&i, sizeof(i));
			switch(iEndiannessMode){
			case Endianness::BigEndian:
				entryParams.szValue = QString::number(qFromBigEndian<qint32>(i));
				break;
			default:
				entryParams.szValue = QString::number(i);
				break;
			}
		}else {
			entryParams.szValue = pItem->m_szValue;
		}
		appendDict(dict, entryParams.szName, entryParams.szValue);
		if(bIsVisible){
			appendEntry(entryParams, pParentItem, entryContext);
		}
	}
		break;
	case FileStructureItem::UINT32:
	{
		quint32 i;
		bRes = fileToRead.read((char*)&i, sizeof(i));

		traceInfos(pItem->m_type, pItem->m_szName, QString("value:%0").arg(i));

		if (pItem->m_szValue.isEmpty()) {
			switch(iEndiannessMode){
			case Endianness::BigEndian:
	#if QT_VERSION_MAJOR >= 5
				entryParams.szValue = QString::number(qFromBigEndian<quint32>(i));
	#else
				entryParams.szValue = QString::number((quint32)qFromBigEndian<qint32>((qint32)i));
	#endif
				break;
			default:
				entryParams.szValue = QString::number(i);
				break;
			}
		}else {
			entryParams.szValue = pItem->m_szValue;
		}
		appendDict(dict, entryParams.szName, entryParams.szValue);
		if(bIsVisible){
			appendEntry(entryParams, pParentItem, entryContext);
		}
	}
		break;
	case FileStructureItem::INT64:
	{
		if (pItem->m_szValue.isEmpty()) {
			qint64 i;
			bRes = fileToRead.read((char*)&i, sizeof(i));
			switch(iEndiannessMode){
			case Endianness::BigEndian:
				entryParams.szValue = QString::number(qFromBigEndian<qint64>(i));
				break;
			default:
				entryParams.szValue = QString::number(i);
				break;
			}
		}else {
			entryParams.szValue = pItem->m_szValue;
		}
		appendDict(dict, entryParams.szName, entryParams.szValue);
		if(bIsVisible){
			appendEntry(entryParams, pParentItem, entryContext);
		}
	}
	break;
	case FileStructureItem::UINT64:
	{
		if (pItem->m_szValue.isEmpty()) {
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
				entryParams.szValue = QString::number(i);
				break;
			}
		}else {
			entryParams.szValue = pItem->m_szValue;
		}
		appendDict(dict, entryParams.szName, entryParams.szValue);
		if(bIsVisible){
			appendEntry(entryParams, pParentItem, entryContext);
		}
	}
		break;
	case FileStructureItem::BYTES:
	{
		traceInfos(pItem->m_type, pItem->m_szName, QString("size:%0 bytes").arg(iSizeExpr));

		if(iSizeExpr > 0){
			bRes = fileToRead.seek(iOffsetStart + iSizeExpr);
			if(bIsVisible){
				appendEntry(entryParams, pParentItem, entryContext);
			}
		}
	}
		break;
	case FileStructureItem::STRING:
	{
		if (pItem->m_szValue.isEmpty()) {
	        if(iSizeExpr > 0){
			    char* szString = new char[iSizeExpr+1];
			    bRes = fileToRead.read((char*)szString, iSizeExpr);
			    szString[iSizeExpr] = '\0';
			    entryParams.szValue = szString;
			    delete[] szString;
	        }else{
			    entryParams.szValue = "";
	        }
		}else {
			entryParams.szValue = pItem->m_szValue;
		}

        traceInfos(pItem->m_type, pItem->m_szName, QString("size:%0 bytes, value: %1").arg(iSizeExpr).arg(entryParams.szValue));

		appendDict(dict, entryParams.szName, entryParams.szValue);
		if(bIsVisible){
			appendEntry(entryParams, pParentItem, entryContext);
		}
	}
		break;
	default:
		break;
	}

	float fProgressValue = (fileToRead.pos() / (float)m_iFileSize);
	m_pFileStructureView->setProgressValue((int)(fProgressValue*100.0));

	QCoreApplication::processEvents();

	traceEnd(pItem->m_type, pItem->m_szName, fileToRead);

	if(!bRes){
		qWarning("[FileStructure] Error to parse structure");
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

#ifdef USE_NO_QREGEXP
	QRegularExpression re("\\$\\{([A-Za-z0-9]+)\\}*");
	QRegularExpressionMatch match;
	qsizetype iCurrentPos = 0;

	QString szVarName;
	QString szDictValue;

	while ((match = re.match(szExpression, iCurrentPos)).hasMatch()) {
		iCurrentPos = match.capturedStart(1);

		if (!match.captured(1).isEmpty()) {
			szVarName = match.captured(1);
			iCurrentPos += (szVarName.size() + 3); // Mise à jour de la position actuelle

			if (dict.contains(szVarName)) {
				szDictValue = dict.value(szVarName);
				szNewExpression = szNewExpression.replace("${" + szVarName + "}", szDictValue);
			}
		}
	}

#else
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
#endif

	szNewExpression = szNewExpression.replace("AND", "&&");
	szNewExpression = szNewExpression.replace("OR", "||");
	szNewExpression = szNewExpression.replace("NOT", "!");
	szNewExpression = szNewExpression.replace("LT", "<");
	szNewExpression = szNewExpression.replace("LE", "<=");
	szNewExpression = szNewExpression.replace("GT", ">");
	szNewExpression = szNewExpression.replace("GE", ">=");

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

void QFileStructureViewController::traceBegin(FileStructureItem::ItemType iItemType, const QString& szName, const QFile& file)
{
	QString szPad = QString("%0").arg(" ", (m_iTraceLevel*2)+1);
	QString szItemType = FileStructureItem::getTypeString(iItemType).toUpper();

	if(false){
		if(szName.isEmpty()){
			qDebug("%03d: %s %s [offset:%lu] {",  m_iTraceLevel, qPrintable(szPad), qPrintable(szItemType), (unsigned long)file.pos());
		}else{
			qDebug("%03d: %s %s [name:%s, offset:%lu] {",  m_iTraceLevel, qPrintable(szPad), qPrintable(szItemType), qPrintable(szName), (unsigned long)file.pos());
		}
	}

	m_iTraceLevel++;
}

void QFileStructureViewController::traceEnd(FileStructureItem::ItemType iItemType, const QString& szName, const QFile& file)
{
	m_iTraceLevel--;

	QString szPad = QString("%0").arg(" ", (m_iTraceLevel*2)+1);
	QString szItemType = FileStructureItem::getTypeString(iItemType).toUpper();

	if(false){
		qDebug("%03d: %s } // %s",  m_iTraceLevel, qPrintable(szPad), qPrintable(szItemType));
		if(szName.isEmpty()){
			//qDebug("%s END: %s offset %lld", qPrintable(szPad), qPrintable(szItemType), (long long int)file.pos());
		}else{
			//qDebug("%s END: %s %s offset %lld", qPrintable(szPad), qPrintable(szItemType), qPrintable(szName), (long long int)file.pos());
		}
	}
}

void QFileStructureViewController::traceInfos(FileStructureItem::ItemType iItemType, const QString& szName, const QString& szMessage)
{
	QString szPad = QString("%0").arg(" ", (m_iTraceLevel*2)+1);
	QString szItemType = FileStructureItem::getTypeString(iItemType).toUpper();

	if(false){
		qDebug("%03d: %s [%s]", m_iTraceLevel, qPrintable(szPad), qPrintable(szMessage));
	}
}
