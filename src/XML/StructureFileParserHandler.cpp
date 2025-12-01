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



bool StructureFileParserHandler::parse(QXmlStreamReader& xmlReader)
{
	bool bRes = true;

	while (!xmlReader.atEnd()) {
		xmlReader.readNext();

		QXmlStreamAttributes attributes = xmlReader.attributes();
		QString qName = xmlReader.name().toString();
		if(xmlReader.isStartElement())
		{
			FileStructureItemSharedPtr pItem;
			FileStructureComplexTypeSharedPtr pComplexType;

			if(qName == "structure_file"){
				// Set version
				QString szVersion = attributes.value("version").toString();
				m_pFileStructure->setVersion(szVersion.toInt());

				// Set endianess
				QString szEndianness = attributes.value("endianness").toString();
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
				QString szName = attributes.value("name").toString();
				QString szType = attributes.value("type").toString();
				QString szSize = attributes.value("size").toString();
				QString szDisplay = attributes.value("display").toString();
				QString szEndianness = attributes.value("endianness").toString();
				FileStructureItem::ItemType iType = getFileStructureItemType(qName, szType);
				if(iType == FileStructureItem::COMPLEXTYPE){
					pComplexType = m_pFileStructure->getComplexType(szType);
					if(pComplexType){
						pItem = FileStructureItem::createFIELD_COMPLEXTYPE(szName, pComplexType);
						pItem->m_szExpr = szSize;
					}else{
						qCritical("[XML] Cannot find complex type '%s' for field '%s'", qPrintable(szType), qPrintable(szName));
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

				// Dispaly mode
				if(szDisplay == "none"){
					pItem->m_iFlags |= FileStructureItem::DisplayNone;
				}
				if(szDisplay == "flat"){
					pItem->m_iFlags |= FileStructureItem::DisplayFlat;
				}

				appendFileStructureItem(pItem, false);
			}

			if(qName == "variable"){
				QString szName = attributes.value("name").toString();
				QString szValueType = attributes.value("type").toString();
				QString szValue = attributes.value("value").toString();
				pItem = FileStructureItem::createVARIABLE(szName);
				pItem->m_szValue = szValue;
				pItem->m_szValueType = szValue;
				appendFileStructureItem(pItem, false);
			}

			if(qName == "block"){
				QString szName = attributes.value("name").toString();
				pItem = FileStructureItem::createBLOCK(szName);
				appendFileStructureItem(pItem, true);
			}

			if(qName == "list"){
				QString szName = attributes.value("name").toString();
				QString szSizeMode = attributes.value("mode").toString();
				QString szSize = attributes.value("size").toString();
				QString szDisplay = attributes.value("display").toString();
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

				// Dispaly mode
				if(szDisplay == "none"){
					pItem->m_iFlags |= FileStructureItem::DisplayNone;
				}
				if(szDisplay == "flat"){
					pItem->m_iFlags |= FileStructureItem::DisplayFlat;
				}

				appendFileStructureItem(pItem, true);
			}

			if(qName == "list_item_infos"){
				QString szExpr = attributes.value("name").toString();
				pItem = FileStructureItem::createLIST_ITEM_INFOS(szExpr);
				appendFileStructureItem(pItem, false);
			}

			if(qName == "condition"){
				QString szExpr = attributes.value("expr").toString();
				pItem = FileStructureItem::createCOND(szExpr);
				appendFileStructureItem(pItem, true);
			}

			if(qName == "complex_type"){
				QString szName = attributes.value("name").toString();
				FileStructureComplexTypeSharedPtr pFileStructureComplexType;
				pFileStructureComplexType = m_pFileStructure->getComplexType(szName);
				if(pFileStructureComplexType.isNull()){
					m_pCurrentComplexType = FileStructureComplexType::create(szName);
					m_pFileStructure->addComplexType(m_pCurrentComplexType);
				}else{
					m_pCurrentComplexType = pFileStructureComplexType;
				}
				m_pCurrentParentItem = m_pCurrentComplexType->getRootItem();
				m_stackCurrentItem.append(m_pCurrentParentItem);
			}

			if(qName == "seek"){
				QString szMode = attributes.value("mode").toString();
				QString szOffset = attributes.value("offset").toString();
				pItem = FileStructureItem::createSEEK(szMode, szOffset);
				appendFileStructureItem(pItem, false);
			}
		}

		if (xmlReader.isEndElement()) {

			if(qName == "list" || qName == "condition" || qName == "block"){
				m_stackCurrentItem.removeLast();
				m_pCurrentParentItem = m_stackCurrentItem.last();
			}

			if(qName == "complex_type"){
				m_pCurrentComplexType.clear();

				m_pCurrentParentItem = m_pFileStructure->getRootItem();
				m_stackCurrentItem.append(m_pCurrentParentItem);
			}
		}
	}

	if (xmlReader.hasError()) {
		qCritical("[XML] Event list parse error: %s", qPrintable(xmlReader.errorString()));
		return false;
	}

	return bRes;
}