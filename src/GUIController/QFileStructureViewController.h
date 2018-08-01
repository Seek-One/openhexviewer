/*
 * QFileStructureViewController.h
 *
 *  Created on: 31 juil. 2018
 *      Author: ebeuque
 */

#ifndef SRC_GUICONTROLLER_QFILESTRUCTUREVIEWCONTROLLER_H_
#define SRC_GUICONTROLLER_QFILESTRUCTUREVIEWCONTROLLER_H_

#include <QObject>

class QFileStructureView;
class QFileStructureModel;

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
	QFileStructureView* m_pFileStructureView;

	QString m_szCurrentFilePath;

	QFileStructureModel* m_pModel;
};

#endif /* SRC_GUICONTROLLER_QFILESTRUCTUREVIEWCONTROLLER_H_ */
