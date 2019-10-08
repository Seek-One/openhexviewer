/*
 * FileStructureItem.h
 *
 *  Created on: 4 oct. 2019
 *      Author: ebeuque
 */

#ifndef SRC_FILESTRUCTURE_FILESTRUCTUREITEM_H_
#define SRC_FILESTRUCTURE_FILESTRUCTUREITEM_H_

#include <QString>
#include <QList>
#include <QSharedPointer>

#include "FileStructureDefinitions.h"

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
		// Complex type
		COMPLEXTYPE,
		// Block
		BLOCK,
		// Collection
		LIST,
		// Condition
		COND,
		// Root
		ROOT
	};

	enum SizeMode {
		ModeBytes = 1,
		ModeCount
	};

    enum ItemFlag {
		LittleEndian = 0x01,
		BigEndian = 0x02,
		FlatList = 0x04,
    };

public:
	FileStructureItem();
	virtual ~FileStructureItem();

	static qint64 getBasicItemTypeSize(FileStructureItem::ItemType type);

	static FileStructureItemSharedPtr createFIELD(const QString& szName, ItemType type, qint64 iSize);
	static FileStructureItemSharedPtr createFIELD_ComplexType(const QString& szName, const FileStructureComplexTypeSharedPtr& pComplexType);
	static FileStructureItemSharedPtr createBLOCK(const QString& szName);
	static FileStructureItemSharedPtr createLIST(const QString& szName);
	static FileStructureItemSharedPtr createCOND(const QString& szExpr);

	void append(const FileStructureItemSharedPtr& pItem);

	QString getTypeString() const;

public:
	QString m_szName;
	ItemType m_type;

	SizeMode m_iSizeMode;
	qint64 m_iSize; // Size in bytes

	FileStructureComplexTypeSharedPtr m_pComplexType;

	int m_iFlags;

	QString m_szExpr; // COND

	QList<FileStructureItemSharedPtr> m_listChildren;
};

#endif /* SRC_FILESTRUCTURE_FILESTRUCTUREITEM_H_ */
