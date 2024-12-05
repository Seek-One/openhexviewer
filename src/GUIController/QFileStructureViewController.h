/*
 * QFileStructureViewController.h
 *
 *  Created on: 31 juil. 2018
 *      Author: ebeuque
 */

#ifndef SRC_GUICONTROLLER_QFILESTRUCTUREVIEWCONTROLLER_H_
#define SRC_GUICONTROLLER_QFILESTRUCTUREVIEWCONTROLLER_H_

#include <QObject>
#include <QString>
#include <QFile>
#include <QHash>
#include <QModelIndex>
#include <QStack>

#include "FileStructure/FileStructureDefinitions.h"
#include "FileStructure/FileStructureItem.h"

class QFileStructureView;
class QFileStructureModel;
class QStandardItem;

class FileStructure;

class EntryParams;
class EntryContext;

typedef QHash<QString, QString> DictVariable;

class QFileStructureViewController : public QObject
{
	Q_OBJECT
public:
	QFileStructureViewController(QFileStructureView* pFileStructureView);
	virtual ~QFileStructureViewController();

	void setCurrentFile(const QString& szFilePath);

	void openStructureFile(const QString& szFilePath);

	void reload();


public slots:
	void openStructureFile();
	
	void loadStructure();
	void entrySelected(const QModelIndex &current, const QModelIndex &previous);

signals:
	void fileStructureItemSelected(qint64 offset, qint64 size);

private slots:
	void reloadStructureFiles();

private:
	void loadStructureFileList(const QString& szOrigin, const QString& szDirPath);

	bool readFileWithStructure(const QString& szFilePath, const FileStructure& loadedFileStructure);

	bool processFileStructureItem(const FileStructureItemSharedPtr& pItem, QFile& fileToRead, DictVariable& dict, QStandardItem* pParentItem, bool bParentVisible);

	void appendEntry(const EntryParams& params, QStandardItem* pParentItem, EntryContext& context);
	void appendDict(DictVariable& dict, const QString& szName, const QString& szValue);

	bool prepareExpr(const QString& szExpression, const DictVariable& dict, QString& szNewExpression);
	bool evaluateBooleanExpr(const QString& szExpression, bool& bError);
	int evaluateIntExpr(const QString& szExpression);

	bool isNumber(const QString& szStr) const;

private:
	QFileStructureView* m_pFileStructureView;

	Endianness::Mode m_iDefaultEndianness;
	QString m_szCurrentFilePath;

	QStack<QString> m_stackListItemInfos; // For now just the list name

	QFileStructureModel* m_pModel;
	qint64 m_iFileSize;

	int m_iTraceLevel;

	void traceBegin(FileStructureItem::ItemType iItemType, const QString& szName, const QFile& file);
	void traceEnd(FileStructureItem::ItemType iItemType, const QString& szName, const QFile& file);
	void traceInfos(FileStructureItem::ItemType iItemType, const QString& szName, const QString& szMessage);
};

#endif /* SRC_GUICONTROLLER_QFILESTRUCTUREVIEWCONTROLLER_H_ */
