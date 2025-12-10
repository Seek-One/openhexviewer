/*
 * FileStructureItem.cpp
 *
 *  Created on: 4 oct. 2019
 *      Author: ebeuque
 */

#include "FileStructureComplexType.h"

#include "FileStructureItem.h"

FileStructureItem::FileStructureItem()
{
	m_type = FileStructureItem::UNKNOWN;
	m_iSizeMode = ModeBytes;
	m_iSeekMode = SeekModeNone;
	m_iSize = -1;
	m_iFlags = 0;
	m_szOffsetExpr = "";
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

FileStructureItemSharedPtr FileStructureItem::createFIELD_COMPLEXTYPE(const QString& szName, const FileStructureComplexTypeSharedPtr& pComplexType)
{
	FileStructureItemSharedPtr pItem = FileStructureItemSharedPtr(new FileStructureItem());
	pItem->m_szName = szName;
	pItem->m_type = FIELDCOMPLEXTYPE;
	pItem->m_iSize = -1;
	pItem->m_pComplexType = pComplexType;
	return pItem;
}

FileStructureItemSharedPtr FileStructureItem::createCOMPLEXTYPE(const QString& szName)
{
	FileStructureItemSharedPtr pItem = FileStructureItemSharedPtr(new FileStructureItem());
	pItem->m_szName = szName;
	pItem->m_type = COMPLEXTYPE;
	pItem->m_iSize = -1;
	return pItem;
}

FileStructureItemSharedPtr FileStructureItem::createVARIABLE(const QString& szName)
{
	FileStructureItemSharedPtr pItem = FileStructureItemSharedPtr(new FileStructureItem());
	pItem->m_szName = szName;
	pItem->m_type = VARIABLE;
	pItem->m_iSize = -1;
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

FileStructureItemSharedPtr FileStructureItem::createLIST(const QString& szName)
{
	FileStructureItemSharedPtr pItem = FileStructureItemSharedPtr(new FileStructureItem());
	pItem->m_szName = szName;
	pItem->m_type = LIST;
	pItem->m_iSizeMode = ModeCount;
	pItem->m_iSize = -1;
	return pItem;
}

FileStructureItemSharedPtr FileStructureItem::createLIST_ITEM_INFOS(const QString& szName)
{
	FileStructureItemSharedPtr pItem = FileStructureItemSharedPtr(new FileStructureItem());
	pItem->m_szExpr = szName;
	pItem->m_type = LIST_ITEM_INFOS;
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

FileStructureItemSharedPtr FileStructureItem::createSEEK(const QString& szMode, const QString& szOffset)
{
	FileStructureItemSharedPtr pItem = FileStructureItemSharedPtr(new FileStructureItem());
	pItem->m_szExpr = szOffset;
	pItem->m_type = SEEK;
	pItem->m_iSeekMode = SeekModeAbsolute;
	if(szMode == "backward"){
		pItem->m_iSeekMode = SeekModeBackward;
	}
	if(szMode == "forward"){
		pItem->m_iSeekMode = SeekModeForward;
	}
	pItem->m_iSize = -1;
	return pItem;
}

void FileStructureItem::append(const FileStructureItemSharedPtr& pItem)
{
	m_listChildren.append(pItem);
}

QString FileStructureItem::getTypeString() const
{
	return getTypeString(m_type);
}

QString FileStructureItem::getTypeString(FileStructureItem::ItemType type)
{
	switch(type){
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
	case BITS: return "bits";
	case VARIABLE: return "variable";
	case FIELDCOMPLEXTYPE: return "field_complex_type";
	case COMPLEXTYPE: return "complex_type";
	case LIST: return "list";
	case LIST_ITEM_INFOS: return "list_item_infos";
	case COND: return "cond";
	case SEEK: return "seek";
	case ROOT: return "root";
	default:
		break;
	}
	return "";
}


bool FileStructureItem::isValueTypeInteger(const QString& szType)
{
	if(szType == "int8"){
		return true;
	}
	if(szType == "uint8"){
		return true;
	}
	if(szType == "int16"){
		return true;
	}
	if(szType == "uint16"){
		return true;
	}
	if(szType == "int32"){
		return true;
	}
	if(szType == "uint32"){
		return true;
	}
	if(szType == "int64"){
		return true;
	}
	if(szType == "uint64"){
		return true;
	}
	return false;
}