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

class QFileStructureViewController : public QObject
{
	Q_OBJECT
public:
	QFileStructureViewController(QFileStructureView* pFileStructureView);
	virtual ~QFileStructureViewController();

public slots:
	void loadStructure();

private:
	QFileStructureView* m_pFileStructureView;
};

#endif /* SRC_GUICONTROLLER_QFILESTRUCTUREVIEWCONTROLLER_H_ */
