/*
 * StructureFileParserHandler.cpp
 *
 *  Created on: 31 juil. 2018
 *      Author: ebeuque
 */

#include "FileStructure/FileStructure.h"

#include "StructureFileParserHandler.h"

StructureFileParserHandler::StructureFileParserHandler(FileStructure* pFileStructure)
{
	m_pFileStructure = pFileStructure;

	m_pCurrentParentItem = pFileStructure->getRootItem();
	m_stackCurrentItem.append(m_pCurrentParentItem);
}

StructureFileParserHandler::~StructureFileParserHandler()
{

}

FileStructureItem::ItemType StructureFileParserHandler::getFileStructureItemType(const QString& szTagName, const QString& szType)
{
	if(szTagName == "field"){
		if(szType == "int8"){
			return FileStructureItem::INT8;
		}
		if(szType == "uint8"){
			return FileStructureItem::UINT8;
		}
		if(szType == "int16"){
			return FileStructureItem::INT16;
		}
		if(szType == "uint16"){
			return FileStructureItem::UINT16;
		}
		if(szType == "int32"){
			return FileStructureItem::INT32;
		}
		if(szType == "uint32"){
			return FileStructureItem::UINT32;
		}
		if(szType == "int64"){
			return FileStructureItem::UINT64;
		}
		if(szType == "uint64"){
			return FileStructureItem::UINT64;
		}
		if(szType == "bytes"){
			return FileStructureItem::BYTES;
		}
		if(szType == "string"){
			return FileStructureItem::STRING;
		}
	}else if(szTagName == "list"){
		return FileStructureItem::LIST;
	}
	return FileStructureItem::UNKNOWN;
}

bool StructureFileParserHandler::startElement(const QString &namespaceURI,
		const QString &localName,
		const QString &qName,
		const QXmlAttributes &attributes)
{
	bool bRes = true;

	FileStructureItemSharedPtr pItem;

	if(qName == "structure_file"){
		// Set version
		QString szVersion = attributes.value("version");
		m_pFileStructure->setVersion(szVersion.toInt());

        // Set endianess
		QString szEndianness = attributes.value("endianness");
		if(!szEndianness.isEmpty()){
			if(szEndianness == "big-endian"){
				m_pFileStructure->setDefaultEndianness(Endianness::BigEndian);
			}
			if(szEndianness == "little-endian"){
				m_pFileStructure->setDefaultEndianness(Endianness::LittleEndian);
			}
		}
	}

	if(qName == "field"){
		QString szName = attributes.value("name");
		QString szType = attributes.value("type");
		QString szSize = attributes.value("size");
		QString szEndianness = attributes.value("endianness");
		FileStructureItem::ItemType iType = getFileStructureItemType(qName, szType);
		qint64 iSize = FileStructureItem::getBasicItemTypeSize(iType);
		pItem = FileStructureItem::createFIELD(szName, iType, iSize);
		pItem->m_szExpr = szSize;
        // Define endianess
		if(!szEndianness.isEmpty()){
			if(szEndianness == "big-endian"){
				pItem->m_iFlags |= FileStructureItem::BigEndian;
			}
			if(szEndianness == "little-endian"){
				pItem->m_iFlags |= FileStructureItem::LittleEndian;
			}
		}
		m_pCurrentParentItem->append(pItem);
	}

	if(qName == "block"){
		QString szName = attributes.value("name");
		pItem = FileStructureItem::createBLOCK(szName);
		m_pCurrentParentItem->append(pItem);
		m_pCurrentParentItem = pItem;
		m_stackCurrentItem.append(m_pCurrentParentItem);
	}

	if(qName == "list"){
		QString szName = attributes.value("name");
		QString szSize = attributes.value("size");
		pItem = FileStructureItem::createLIST(szName, -1);
		pItem->m_szExpr = szSize;
		m_pCurrentParentItem->append(pItem);
		m_pCurrentParentItem = pItem;
		m_stackCurrentItem.append(m_pCurrentParentItem);
	}

	if(qName == "condition"){
		QString szExpr = attributes.value("expr");
		pItem = FileStructureItem::createCOND(szExpr);
		m_pCurrentParentItem->append(pItem);
		m_pCurrentParentItem = pItem;
		m_stackCurrentItem.append(m_pCurrentParentItem);
	}

	return bRes;
}

bool StructureFileParserHandler::endElement(const QString &namespaceURI,
		const QString &localName,
		const QString &qName)
{
	bool bRes = true;

	if(qName == "list" || qName == "condition" || qName == "block"){
		m_stackCurrentItem.removeLast();
		m_pCurrentParentItem = m_stackCurrentItem.last();
	}

	return bRes;
}

bool StructureFileParserHandler::characters(const QString &str)
{
	return true;
}

bool StructureFileParserHandler::fatalError(const QXmlParseException &exception)
{
	return false;
}
