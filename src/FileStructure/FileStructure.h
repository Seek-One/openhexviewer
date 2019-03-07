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

class Endianness
{
public:
	enum Mode {
		Auto = 0,
		LittleEndian,
		BigEndian
	};
};

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
		// Bytes type
		BYTES,
		STRING,
		// Block
		BLOCK,
		// Collection
		LIST,
		// Condition
		COND,
		// Root
		ROOT
	};

    enum ItemFlag {
		LittleEndian = 0x01,
		BigEndian = 0x02,
    };

public:
	FileStructureItem();
	virtual ~FileStructureItem();

	static qint64 getBasicItemTypeSize(FileStructureItem::ItemType type);

	static FileStructureItemSharedPtr createFIELD(const QString& szName, ItemType type, qint64 iSize);
	static FileStructureItemSharedPtr createBLOCK(const QString& szName);
	static FileStructureItemSharedPtr createLIST(const QString& szName, qint64 iSize);
	static FileStructureItemSharedPtr createCOND(const QString& szExpr);

	void append(const FileStructureItemSharedPtr& pItem);

	QString getTypeString() const;

public:
	QString m_szName;
	ItemType m_type;
	qint64 m_iSize;
	int m_iFlags;

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

	void setDefaultEndianness(Endianness::Mode iMode);
	Endianness::Mode getDefaultEndianness() const;

	FileStructureItemSharedPtr getRootItem() const;

private:
	int m_iVersion;
	Endianness::Mode m_iDefaultEndianness;

	FileStructureItemSharedPtr m_pRootItem;
};

#endif /* SRC_FILESTRUCTURE_FILESTRUCTURE_H_ */
