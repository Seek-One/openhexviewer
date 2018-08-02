/*
 * QFileStructureView.h
 *
 *  Created on: 31 juil. 2018
 *      Author: ebeuque
 */

#ifndef SRC_GUI_QFILESTRUCTUREVIEW_H_
#define SRC_GUI_QFILESTRUCTUREVIEW_H_

#include <QWidget>

class QTreeView;
class QPushButton;
class QAbstractItemModel;

class QFileStructureView : public QWidget
{
	Q_OBJECT
public:
	QFileStructureView(QWidget* pParent = NULL);
	virtual ~QFileStructureView();

	QTreeView* getTreeview() const;

	QPushButton* getLoadButton() const;

	void setModel(QAbstractItemModel* pItemModel);

private:
	QTreeView* m_pTreeView;

	QPushButton* m_pLoadButton;
};

#endif /* SRC_GUI_QFILESTRUCTUREVIEW_H_ */
