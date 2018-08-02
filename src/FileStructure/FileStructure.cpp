/*
 * FileStructure.cpp
 *
 *  Created on: 2 août 2018
 *      Author: ebeuque
 */

#include "FileStructure.h"

FileStructureItem::FileStructureItem()
{
	m_type = FileStructureItem::UNKNOWN;
	m_iSize = -1;
}

FileStructureItem::~FileStructureItem()
{

}


qint64 FileStructureItem::getBasicItemTypeSize(FileStructureItem::ItemType type)
{
	switch(type){
	case INT8:
	case UINT8:
		return 1;
	case INT16:
	case UINT16:
		return 2;
	case INT32:
	case UINT32:
		return 4;
	case INT64:
	case UINT64:
		return 8;
	default:
		break;
	}
	return -1;

}

FileStructureItemSharedPtr FileStructureItem::createFIELD(const QString& szName, ItemType type, qint64 iSize)
{
	FileStructureItemSharedPtr pItem = FileStructureItemSharedPtr(new FileStructureItem());
	pItem->m_szName = szName;
	pItem->m_type = type;
	pItem->m_iSize = iSize;
	return pItem;
}

FileStructureItemSharedPtr FileStructureItem::createLIST(const QString& szName, qint64 iSize)
{
	FileStructureItemSharedPtr pItem = FileStructureItemSharedPtr(new FileStructureItem());
	pItem->m_szName = szName;
	pItem->m_type = LIST;
	pItem->m_iSize = iSize;
	return pItem;
}

void FileStructureItem::append(const FileStructureItemSharedPtr& pItem)
{
	m_listChildren.append(pItem);
}

FileStructure::FileStructure()
{
	m_iVersion = -1;

	m_pRootItem = QSharedPointer<FileStructureItem>(new FileStructureItem());
	m_pRootItem->m_szName = "root";
	m_pRootItem->m_type = FileStructureItem::ROOT;
	m_pRootItem->m_iSize = -1;
}

FileStructure::~FileStructure()
{

}

void FileStructure::setVersion(int iVersion)
{
	m_iVersion = iVersion;
}

int FileStructure::getVersion() const
{
	return m_iVersion;
}

FileStructureItemSharedPtr FileStructure::getRootItem() const
{
	return m_pRootItem;
}
