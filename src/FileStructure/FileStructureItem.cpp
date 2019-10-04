/*
 * FileStructureItem.cpp
 *
 *  Created on: 4 oct. 2019
 *      Author: ebeuque
 */

#include "FileStructureItem.h"

FileStructureItem::FileStructureItem()
{
	m_type = FileStructureItem::UNKNOWN;
	m_iSize = -1;
	m_iFlags = 0;
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

FileStructureItemSharedPtr FileStructureItem::createFIELD_ComplexType(const QString& szName, const FileStructureComplexTypeSharedPtr& pComplexType)
{
	FileStructureItemSharedPtr pItem = FileStructureItemSharedPtr(new FileStructureItem());
	pItem->m_szName = szName;
	pItem->m_type = COMPLEXTYPE;
	pItem->m_iSize = -1;
	pItem->m_pComplexType = pComplexType;
	return pItem;
}

FileStructureItemSharedPtr FileStructureItem::createBLOCK(const QString& szName)
{
	FileStructureItemSharedPtr pItem = FileStructureItemSharedPtr(new FileStructureItem());
	pItem->m_szName = szName;
	pItem->m_type = BLOCK;
	pItem->m_iSize = -1;
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

FileStructureItemSharedPtr FileStructureItem::createCOND(const QString& szExpr)
{
	FileStructureItemSharedPtr pItem = FileStructureItemSharedPtr(new FileStructureItem());
	pItem->m_szExpr = szExpr;
	pItem->m_type = COND;
	pItem->m_iSize = -1;
	return pItem;
}

void FileStructureItem::append(const FileStructureItemSharedPtr& pItem)
{
	m_listChildren.append(pItem);
}

QString FileStructureItem::getTypeString() const
{
	switch(m_type){
	case INT8: return "int8";
	case UINT8: return "int8";
	case INT16: return "int16";
	case UINT16: return "uint16";
	case INT32: return "int32";
	case UINT32: return "uint32";
	case INT64: return "int64";
	case UINT64: return "uint64";
	case BYTES: return "bytes";
	case STRING: return "string";
	case LIST: return "list";
	case COND: return "cond";
	case ROOT: return "root";
	default:
		break;
	}
	return "";
}
