/*
 * QFileStructureViewController.h
 *
 *  Created on: 31 juil. 2018
 *      Author: ebeuque
 */

#ifndef SRC_GUICONTROLLER_QFILESTRUCTUREVIEWCONTROLLER_H_
#define SRC_GUICONTROLLER_QFILESTRUCTUREVIEWCONTROLLER_H_

#include <QObject>
#include <QString>
#include <QFile>

#include "FileStructure/FileStructure.h"

class QFileStructureView;
class QFileStructureModel;
class QStandardItem;

class FileStructure;

class QFileStructureViewController : public QObject
{
	Q_OBJECT
public:
	QFileStructureViewController(QFileStructureView* pFileStructureView);
	virtual ~QFileStructureViewController();

	void setCurrentFile(const QString& szFilePath);

public slots:
	void loadStructure();

private:
	bool readFileWithStructure(const QString& szFilePath, const FileStructure& loadedFileStructure);

	bool processFileStructureItem(const FileStructureItemSharedPtr& pItem, QFile& fileToRead, QStandardItem* pParentItem);

	QStandardItem* appendEntry(const QString& szName, const QString& szType, const QString& szSize, const QString& szOffsetStart, const QString& szValue, QStandardItem* pParentItem);

private:
	QFileStructureView* m_pFileStructureView;

	QString m_szCurrentFilePath;

	QFileStructureModel* m_pModel;
};

#endif /* SRC_GUICONTROLLER_QFILESTRUCTUREVIEWCONTROLLER_H_ */
