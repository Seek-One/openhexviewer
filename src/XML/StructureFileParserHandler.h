/*
 * StructureFileParserHandler.h
 *
 *  Created on: 31 juil. 2018
 *      Author: ebeuque
 */

#ifndef SRC_XML_STRUCTUREFILEPARSERHANDLER_H_
#define SRC_XML_STRUCTUREFILEPARSERHANDLER_H_

#include <QFile>
#include <QXmlDefaultHandler>

#include "FileStructure/FileStructure.h"

class StructureFileParserHandler : public QXmlDefaultHandler {
public:
	StructureFileParserHandler(FileStructure* pFileStructure);
	virtual ~StructureFileParserHandler();

	virtual bool startElement(const QString &namespaceURI,
			const QString &localName,
			const QString &qName,
			const QXmlAttributes &attributes);
	virtual bool endElement(const QString &namespaceURI,
			const QString &localName,
			const QString &qName);
	virtual bool characters(const QString &str);
	virtual bool fatalError(const QXmlParseException &exception);

private:
	FileStructure* m_pFileStructure;

	FileStructureItemSharedPtr m_pCurrentParentItem;
	QList<FileStructureItemSharedPtr> m_stackCurrentItem;

	FileStructureItem::ItemType getFileStructureItemType(const QString& szTagName, const QString& szType);
};

#endif /* SRC_XML_STRUCTUREFILEPARSERHANDLER_H_ */
