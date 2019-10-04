/*
 * FileStructure.h
 *
 *  Created on: 2 août 2018
 *      Author: ebeuque
 */

#ifndef SRC_FILESTRUCTURE_FILESTRUCTURE_H_
#define SRC_FILESTRUCTURE_FILESTRUCTURE_H_

#include <QString>
#include <QList>
#include <QSharedPointer>

#include "FileStructure/FileStructureDefinitions.h"
#include "FileStructure/FileStructureComplexType.h"

class FileStructure
{
public:
	FileStructure();
	virtual ~FileStructure();

	void setVersion(int iVersion);
	int getVersion() const;

	void setDefaultEndianness(Endianness::Mode iMode);
	Endianness::Mode getDefaultEndianness() const;

	FileStructureItemSharedPtr getRootItem() const;

	void addComplexType(const FileStructureComplexTypeSharedPtr& pComplexType);
	FileStructureComplexTypeSharedPtr getComplexType(const QString& szName) const;

private:
	int m_iVersion;
	Endianness::Mode m_iDefaultEndianness;

	FileStructureItemSharedPtr m_pRootItem;

	FileStructureComplexTypeList m_listComplexType;
};

#endif /* SRC_FILESTRUCTURE_FILESTRUCTURE_H_ */
