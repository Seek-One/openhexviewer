/*
 * FileStructure.cpp
 *
 *  Created on: 2 août 2018
 *      Author: ebeuque
 */

#include "FileStructure/FileStructureItem.h"
#include "FileStructure/FileStructureComplexType.h"


#include "FileStructure.h"

FileStructure::FileStructure()
{
	m_iVersion = -1;
	m_iDefaultEndianness = Endianness::Auto;

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

void FileStructure::setDefaultEndianness(Endianness::Mode iMode)
{
	m_iDefaultEndianness = iMode;
}

Endianness::Mode FileStructure::getDefaultEndianness() const
{
	return m_iDefaultEndianness;
}

FileStructureItemSharedPtr FileStructure::getRootItem() const
{
	return m_pRootItem;
}

void FileStructure::addComplexType(const FileStructureComplexTypeSharedPtr& pComplexType)
{
	m_listComplexType.insert(pComplexType->getName(), pComplexType);
}

FileStructureComplexTypeSharedPtr FileStructure::getComplexType(const QString& szName) const
{
	if(m_listComplexType.contains(szName)){
		return m_listComplexType.value(szName);
	}
	return FileStructureComplexTypeSharedPtr();
}
