/*
 * FileStructureComplexType.h
 *
 *  Created on: 4 oct. 2019
 *      Author: ebeuque
 */

#ifndef SRC_FILESTRUCTURE_FILESTRUCTURECOMPLEXTYPE_H_
#define SRC_FILESTRUCTURE_FILESTRUCTURECOMPLEXTYPE_H_

#include <QHash>
#include <QString>

#include "FileStructureDefinitions.h"

class FileStructureComplexType
{
public:
	FileStructureComplexType(const QString& szName);
	virtual ~FileStructureComplexType();

	static FileStructureComplexTypeSharedPtr create(const QString& szName);

	void setName(const QString& szName);
	QString getName() const;

	FileStructureItemSharedPtr getRootItem() const;

private:
	QString m_szName;

	FileStructureItemSharedPtr m_pRootItem;
};

class FileStructureComplexTypeList : public QHash<QString, FileStructureComplexTypeSharedPtr>
{
public:
	FileStructureComplexTypeList();
	virtual ~FileStructureComplexTypeList();
};

#endif /* SRC_FILESTRUCTURE_FILESTRUCTURECOMPLEXTYPE_H_ */
