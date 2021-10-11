/*
 * StructureFileParserHandler.h
 *
 *  Created on: 31 juil. 2018
 *      Author: ebeuque
 */

#ifndef SRC_XML_STRUCTUREFILEPARSERHANDLER_H_
#define SRC_XML_STRUCTUREFILEPARSERHANDLER_H_

#include <QFile>
#include <QXmlStreamReader>

#include "FileStructure/FileStructure.h"

class StructureFileParserHandler {
public:
	StructureFileParserHandler(FileStructure* pFileStructure);
	virtual ~StructureFileParserHandler();

	bool parse(QXmlStreamReader& xmlReader);

private:
	FileStructure* m_pFileStructure;

	// FileStructureType building
	FileStructureComplexTypeSharedPtr m_pCurrentComplexType;

	// FileStructure building
	FileStructureItemSharedPtr m_pCurrentParentItem;
	QList<FileStructureItemSharedPtr> m_stackCurrentItem;

	FileStructureItem::ItemType getFileStructureItemType(const QString& szTagName, const QString& szType);

	void appendFileStructureItem(const FileStructureItemSharedPtr& pItem, bool bUpdateParent);
};

#endif /* SRC_XML_STRUCTUREFILEPARSERHANDLER_H_ */
