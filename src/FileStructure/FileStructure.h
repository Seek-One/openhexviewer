/*
 * FileStructure.h
 *
 *  Created on: 2 août 2018
 *      Author: ebeuque
 */

#ifndef SRC_FILESTRUCTURE_FILESTRUCTURE_H_
#define SRC_FILESTRUCTURE_FILESTRUCTURE_H_

#include <QString>
#include <QList>
#include <QSharedPointer>

class FileStructureItem;
typedef QSharedPointer<FileStructureItem> FileStructureItemSharedPtr;

class FileStructureItem
{
public:
	enum ItemType {
		UNKNOWN,
		// Integer type
		INT8,
		UINT8,
		INT16,
		UINT16,
		INT32,
		UINT32,
		INT64,
		UINT64,
		// Collection
		LIST,
		// Condition
		COND,
		// Root
		ROOT
	};

public:
	FileStructureItem();
	virtual ~FileStructureItem();

	static qint64 getBasicItemTypeSize(FileStructureItem::ItemType type);

	static FileStructureItemSharedPtr createFIELD(const QString& szName, ItemType type, qint64 iSize);
	static FileStructureItemSharedPtr createLIST(const QString& szName, qint64 iSize);
	static FileStructureItemSharedPtr createCOND(const QString& szExpr);

	void append(const FileStructureItemSharedPtr& pItem);

	QString getTypeString() const;

public:
	QString m_szName;
	ItemType m_type;
	qint64 m_iSize;

	QString m_szExpr; // COND

	QList<FileStructureItemSharedPtr> m_listChildren;
};

class FileStructure
{
public:
	FileStructure();
	virtual ~FileStructure();

	void setVersion(int iVersion);
	int getVersion() const;

	FileStructureItemSharedPtr getRootItem() const;

private:
	int m_iVersion;

	FileStructureItemSharedPtr m_pRootItem;
};

#endif /* SRC_FILESTRUCTURE_FILESTRUCTURE_H_ */
