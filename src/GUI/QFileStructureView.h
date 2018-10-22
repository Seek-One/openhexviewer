/*
 * QFileStructureView.h
 *
 *  Created on: 31 juil. 2018
 *      Author: ebeuque
 */

#ifndef SRC_GUI_QFILESTRUCTUREVIEW_H_
#define SRC_GUI_QFILESTRUCTUREVIEW_H_

#include <QWidget>

class QComboBox;
class QTreeView;
class QPushButton;
class QAbstractItemModel;

class QFileStructureView : public QWidget
{
	Q_OBJECT
public:
	QFileStructureView(QWidget* pParent = NULL);
	virtual ~QFileStructureView();

	QComboBox* getStructureFileComboBox() const;
	QTreeView* getTreeview() const;

	QPushButton* getLoadButton() const;

	void setModel(QAbstractItemModel* pItemModel);

private:
	QComboBox* m_pStructureFileComboBox;
	QTreeView* m_pTreeView;

	QPushButton* m_pLoadButton;
};

#endif /* SRC_GUI_QFILESTRUCTUREVIEW_H_ */
