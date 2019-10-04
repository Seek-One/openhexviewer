/*
 * FileStructureComplexType.cpp
 *
 *  Created on: 4 oct. 2019
 *      Author: ebeuque
 */

#include "FileStructure/FileStructureItem.h"

#include "FileStructureComplexType.h"

FileStructureComplexType::FileStructureComplexType(const QString& szName)
{
	m_szName = szName;
	m_pRootItem = FileStructureItem::createBLOCK(szName);
}

FileStructureComplexType::~FileStructureComplexType()
{

}

FileStructureComplexTypeSharedPtr FileStructureComplexType::create(const QString& szName)
{
	FileStructureComplexType* pTmp = new FileStructureComplexType(szName);
	return FileStructureComplexTypeSharedPtr(pTmp);
}


void FileStructureComplexType::setName(const QString& szName)
{
	m_szName = szName;
}

QString FileStructureComplexType::getName() const
{
	return m_szName;
}

FileStructureItemSharedPtr FileStructureComplexType::getRootItem() const
{
	return m_pRootItem;
}

FileStructureComplexTypeList::FileStructureComplexTypeList()
{

}

FileStructureComplexTypeList::~FileStructureComplexTypeList()
{

}
