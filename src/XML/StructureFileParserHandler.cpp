/*
 * StructureFileParserHandler.cpp
 *
 *  Created on: 31 juil. 2018
 *      Author: ebeuque
 */

#include "GUIModel/QFileStructureModel.h"

#include "StructureFileParserHandler.h"

StructureFileParserHandler::StructureFileParserHandler(const QString& szFilePath, QFileStructureModel* pModel)
{
	m_pModel = pModel;
	m_file.setFileName(szFilePath);
}

StructureFileParserHandler::~StructureFileParserHandler()
{

}

int StructureFileParserHandler::getSizeFromType(const QString& szType)
{
	if(szType == "int8"){
		return 1;
	}
	if(szType == "uint8"){
		return 1;
	}
	if(szType == "int16"){
		return 2;
	}
	if(szType == "uint16"){
		return 2;
	}
	if(szType == "int32"){
		return 4;
	}
	if(szType == "uint32"){
		return 4;
	}
	if(szType == "int64"){
		return 8;
	}
	if(szType == "uint64"){
		return 8;
	}
	return 0;
}

bool StructureFileParserHandler::readField(const QString& szType, QString& szValue)
{
	bool bRes = false;

	if(szType == "int8"){
		qint8 i;
		bRes = m_file.read((char*)&i, sizeof(i));
		szValue = QString::number(i);
		return bRes;
	}
	if(szType == "uint8"){
		quint8 i;
		bRes = m_file.read((char*)&i, sizeof(i));
		szValue = QString::number(i);
		return bRes;
	}
	if(szType == "int16"){
		qint16 i;
		bRes = m_file.read((char*)&i, sizeof(i));
		szValue = QString::number(i);
		return bRes;
	}
	if(szType == "uint16"){
		quint16 i;
		bRes = m_file.read((char*)&i, sizeof(i));
		szValue = QString::number(i);
		return bRes;
	}
	if(szType == "int32"){
		qint32 i;
		bRes = m_file.read((char*)&i, sizeof(i));
		szValue = QString::number(i);
		return bRes;
	}
	if(szType == "uint32"){
		quint32 i;
		bRes = m_file.read((char*)&i, sizeof(i));
		szValue = QString::number(i);
		return bRes;
	}
	if(szType == "int64"){
		qint64 i;
		bRes = m_file.read((char*)&i, sizeof(i));
		szValue = QString::number(i);
		return bRes;
	}
	if(szType == "uint64"){
		quint64 i;
		bRes = m_file.read((char*)&i, sizeof(i));
		szValue = QString::number(i);
		return bRes;
	}
	return bRes;
}

void StructureFileParserHandler::appendEntry(const QString& szName, const QString& szType, const QString& szSize, const QString& szOffsetStart, const QString& szValue)
{
	QList<QStandardItem*> listColumns;
	listColumns.append(new QStandardItem(szName));
	listColumns.append(new QStandardItem(szType));
	listColumns.append(new QStandardItem(szSize));
	listColumns.append(new QStandardItem(szOffsetStart));
	listColumns.append(new QStandardItem(szValue));

	if(m_listItem.count() == 0){
		m_pModel->appendRow(listColumns);
	}else{
		QStandardItem* pItem = m_listItem.last();
		pItem->appendRow(listColumns);
	}
}

bool StructureFileParserHandler::startElement(const QString &namespaceURI,
		const QString &localName,
		const QString &qName,
		const QXmlAttributes &attributes)
{
	bool bRes = true;

	if(qName == "structure_file"){
		bRes = m_file.open(QIODevice::ReadOnly);
		if(!bRes){
			qCritical("[File] Cannot open file");
		}
	}

	if(qName == "field"){
		QString szName = attributes.value("name");
		QString szType = attributes.value("type");
		int iSize = getSizeFromType(szType);
		qint64 iOffset = m_file.pos();
		QString szValue;

		bRes = readField(szType, szValue);

		appendEntry(szName, szType, QString::number(iSize), QString::number(iOffset), szValue);
	}

	if(qName == "list"){

	}

	return bRes;
}

bool StructureFileParserHandler::endElement(const QString &namespaceURI,
		const QString &localName,
		const QString &qName)
{
	bool bRes = true;

	if(qName == "structure_file"){
		if(m_file.isOpen()){
			m_file.close();
		}
	}

	return bRes;
}

bool StructureFileParserHandler::characters(const QString &str)
{
	return true;
}

bool StructureFileParserHandler::fatalError(const QXmlParseException &exception)
{
	return true;
}
