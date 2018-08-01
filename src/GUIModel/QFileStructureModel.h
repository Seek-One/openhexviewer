/*
 * QFileStructureModel.h
 *
 *  Created on: 31 juil. 2018
 *      Author: ebeuque
 */

#ifndef SRC_GUIMODEL_QFILESTRUCTUREMODEL_H_
#define SRC_GUIMODEL_QFILESTRUCTUREMODEL_H_

#include <QStandardItemModel>

class QFileStructureModel : public QStandardItemModel
{
public:
	QFileStructureModel();
	virtual ~QFileStructureModel();

/*
protected:
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	*/
};

#endif /* SRC_GUIMODEL_QFILESTRUCTUREMODEL_H_ */
