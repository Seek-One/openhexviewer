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
		FIELDCOMPLEXTYPE,
		COMPLEXTYPE,
		// Block
		BLOCK,
		// Collection
		LIST,
		LIST_ITEM_INFOS,
		// Condition
		COND,
		// Offset,
		SEEK,
		// Root
		ROOT
	};

	enum SizeMode {
		ModeBytes = 1,
		ModeCount
	};

    enum ItemFlag {
    	// Endianess
		LittleEndian = 0x01,
		BigEndian = 0x02,
		// Display
		DisplayNone = 0x04,
		DisplayFlatList = 0x08,
    };

	enum SeekMode {
		SeekModeNone = 0,
		SeekModeAbsolute = 1,
		SeekModeBackward = 2,
		SeekModeForward = 3
	};


public:
	FileStructureItem();
	virtual ~FileStructureItem();

	static qint64 getBasicItemTypeSize(FileStructureItem::ItemType type);

	static FileStructureItemSharedPtr createFIELD(const QString& szName, ItemType type, qint64 iSize);
	static FileStructureItemSharedPtr createFIELD_COMPLEXTYPE(const QString& szName, const FileStructureComplexTypeSharedPtr& pComplexType);
	static FileStructureItemSharedPtr createCOMPLEXTYPE(const QString& szName);
	static FileStructureItemSharedPtr createBLOCK(const QString& szName);
	static FileStructureItemSharedPtr createLIST(const QString& szName);
	static FileStructureItemSharedPtr createLIST_ITEM_INFOS(const QString& szName);
	static FileStructureItemSharedPtr createCOND(const QString& szExpr);
	static FileStructureItemSharedPtr createSEEK(const QString& szMode, const QString& szSize);

	void append(const FileStructureItemSharedPtr& pItem);

	QString getTypeString() const;
	static QString getTypeString(FileStructureItem::ItemType type);

public:
	QString m_szName;
	ItemType m_type;

	SizeMode m_iSizeMode;
	qint64 m_iSize; // Size in bytes

	SeekMode m_iSeekMode;

	FileStructureComplexTypeSharedPtr m_pComplexType;

	int m_iFlags;

	QString m_szExpr; // COND

	QList<FileStructureItemSharedPtr> m_listChildren;
};

#endif /* SRC_FILESTRUCTURE_FILESTRUCTUREITEM_H_ */
