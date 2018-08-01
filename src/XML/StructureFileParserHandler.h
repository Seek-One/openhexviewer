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

class QFileStructureModel;

class StructureFileParserHandler : public QXmlDefaultHandler {
public:
	StructureFileParserHandler(const QString& szFilePath, QFileStructureModel* pModel);
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
	QFile m_file;
	QFileStructureModel* m_pModel;
	QList<QStandardItem*> m_listItem;

	int getSizeFromType(const QString& szType);
	bool readField(const QString& szType, QString& szValue);

	void appendEntry(const QString& szName, const QString& szType, const QString& szSize, const QString& szOffsetStart, const QString& szValue);

};

#endif /* SRC_XML_STRUCTUREFILEPARSERHANDLER_H_ */
