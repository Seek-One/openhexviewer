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

class EntryParams;
class EntryContext;

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

	void appendEntry(const EntryParams& params, QStandardItem* pParentItem, EntryContext& context);

private:
	QFileStructureView* m_pFileStructureView;

	QString m_szCurrentFilePath;

	QFileStructureModel* m_pModel;
};

#endif /* SRC_GUICONTROLLER_QFILESTRUCTUREVIEWCONTROLLER_H_ */
