/*
 * FileStructureDefinitions.h
 *
 *  Created on: 4 oct. 2019
 *      Author: ebeuque
 */

#ifndef SRC_FILESTRUCTURE_FILESTRUCTUREDEFINITIONS_H_
#define SRC_FILESTRUCTURE_FILESTRUCTUREDEFINITIONS_H_

#include <QSharedPointer>

class Endianness
{
public:
	enum Mode {
		Auto = 0,
		LittleEndian,
		BigEndian
	};
};

class FileStructureItem;
typedef QSharedPointer<FileStructureItem> FileStructureItemSharedPtr;

class FileStructureComplexType;
typedef QSharedPointer<FileStructureComplexType> FileStructureComplexTypeSharedPtr;


#endif /* SRC_FILESTRUCTURE_FILESTRUCTUREDEFINITIONS_H_ */
