/*
 * StructureFileParserHandler.cpp
 *
 *  Created on: 31 juil. 2018
 *      Author: ebeuque
 */

#include "FileStructure/FileStructure.h"
#include "FileStructure/FileStructureItem.h"
#include "FileStructure/FileStructureComplexType.h"

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
		if(!szType.isEmpty()){
			return FileStructureItem::COMPLEXTYPE;
		}
	}else if(szTagName == "list"){
		return FileStructureItem::LIST;
	}
	return FileStructureItem::UNKNOWN;
}

void StructureFileParserHandler::appendFileStructureItem(const FileStructureItemSharedPtr& pItem, bool bUpdateParent)
{
	m_pCurrentParentItem->append(pItem);
	if(bUpdateParent){
		m_pCurrentParentItem = pItem;
		m_stackCurrentItem.append(m_pCurrentParentItem);
	}
}

bool StructureFileParserHandler::startElement(const QString &namespaceURI,
		const QString &localName,
		const QString &qName,
		const QXmlAttributes &attributes)
{
	bool bRes = true;

	FileStructureItemSharedPtr pItem;
	FileStructureComplexTypeSharedPtr pComplexType;

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
		if(iType == FileStructureItem::COMPLEXTYPE){
			pComplexType = m_pFileStructure->getComplexType(szType);
			if(pComplexType){
				pItem = FileStructureItem::createFIELD_ComplexType(szName, pComplexType);
			}else{
				qCritical("[XML] Cannot find complex type: %s", qPrintable(szName));
				bRes = false;
			}
		}else{
			qint64 iSize = FileStructureItem::getBasicItemTypeSize(iType);
			pItem = FileStructureItem::createFIELD(szName, iType, iSize);
			pItem->m_szExpr = szSize;
		}
        // Define endianess
		if(!szEndianness.isEmpty()){
			if(szEndianness == "big-endian"){
				pItem->m_iFlags |= FileStructureItem::BigEndian;
			}
			if(szEndianness == "little-endian"){
				pItem->m_iFlags |= FileStructureItem::LittleEndian;
			}
		}

		appendFileStructureItem(pItem, false);
	}

	if(qName == "block"){
		QString szName = attributes.value("name");
		pItem = FileStructureItem::createBLOCK(szName);
		appendFileStructureItem(pItem, true);
	}

	if(qName == "list"){
		QString szName = attributes.value("name");
		QString szSizeMode = attributes.value("mode");
		QString szSize = attributes.value("size");
		QString szDisplay = attributes.value("display");
		pItem = FileStructureItem::createLIST(szName);
		if(!szSizeMode.isEmpty()){
			if(szSizeMode == "bytes"){
				pItem->m_iSizeMode = FileStructureItem::ModeBytes;
			}else{
				pItem->m_iSizeMode = FileStructureItem::ModeCount;
			}
		}else{
			pItem->m_iSizeMode = FileStructureItem::ModeCount;
		}
		pItem->m_szExpr = szSize;

		if(szDisplay == "flat"){
			pItem->m_iFlags |= FileStructureItem::FlatList;
		}

		appendFileStructureItem(pItem, true);
	}

	if(qName == "list_item_infos"){
		QString szExpr = attributes.value("name");
		pItem = FileStructureItem::createLIST_ITEM_INFOS(szExpr);
		appendFileStructureItem(pItem, false);
	}

	if(qName == "condition"){
		QString szExpr = attributes.value("expr");
		pItem = FileStructureItem::createCOND(szExpr);
		appendFileStructureItem(pItem, true);
	}

	if(qName == "complex_type"){
		QString szName = attributes.value("name");
		m_pCurrentComplexType = FileStructureComplexType::create(szName);
		m_pCurrentParentItem = m_pCurrentComplexType->getRootItem();
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

	if(qName == "complex_type"){
		m_pFileStructure->addComplexType(m_pCurrentComplexType);
		m_pCurrentComplexType.clear();

		m_pCurrentParentItem = m_pFileStructure->getRootItem();
		m_stackCurrentItem.append(m_pCurrentParentItem);
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
