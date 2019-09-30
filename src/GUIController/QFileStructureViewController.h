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
#include <QHash>
#include <QModelIndex>

#include "FileStructure/FileStructure.h"

class QFileStructureView;
class QFileStructureModel;
class QStandardItem;

class FileStructure;

class EntryParams;
class EntryContext;

typedef QHash<QString, QString> DictVariable;

class QFileStructureViewController : public QObject
{
	Q_OBJECT
public:
	QFileStructureViewController(QFileStructureView* pFileStructureView);
	virtual ~QFileStructureViewController();

	void setCurrentFile(const QString& szFilePath);

public slots:
	void loadStructure();
	void entrySelected(const QModelIndex &current, const QModelIndex &previous);

signals:
	void fileStructureItemSelected(qint64 offset, qint64 size);

private slots:
	void reloadStructureFiles();

private:
	void loadStructureFileList(const QString& szDirPath);

	bool readFileWithStructure(const QString& szFilePath, const FileStructure& loadedFileStructure);

	bool processFileStructureItem(const FileStructureItemSharedPtr& pItem, QFile& fileToRead, DictVariable& dict, QStandardItem* pParentItem);

	void appendEntry(const EntryParams& params, QStandardItem* pParentItem, EntryContext& context);
	void appendDict(DictVariable& dict, const QString& szName, const QString& szValue);

	bool prepareExpr(const QString& szExpression, const DictVariable& dict, QString& szNewExpression);
	bool evaluateBooleanExpr(const QString& szExpression);
	int evaluateIntExpr(const QString& szExpression);

private:
	QFileStructureView* m_pFileStructureView;

	Endianness::Mode m_iDefaultEndianness;
	QString m_szCurrentFilePath;

	QFileStructureModel* m_pModel;
};

#endif /* SRC_GUICONTROLLER_QFILESTRUCTUREVIEWCONTROLLER_H_ */
