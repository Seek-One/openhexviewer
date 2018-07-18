/*
 * QFileView.h
 *
 *  Created on: 18 juil. 2018
 *      Author: ebeuque
 */

#ifndef SRC_GUI_QFILEVIEW_H_
#define SRC_GUI_QFILEVIEW_H_

#include <QWidget>

class QTextEdit;
class QScrollBar;

class QFileView : public QWidget
{
public:
	QFileView(QWidget* pParent = NULL);
	virtual ~QFileView();

	void setRowCount(int iMax);
	void setCurrentRow(int iRow);

	int getBytePerLine() const;
	int getVisibleRowCount() const;

	void setHexText(const QString& szText);
	void setHumanText(const QString& szText);

private:
	QTextEdit* m_pOffsetEditor;
	QTextEdit* m_pHexEditor;
	QTextEdit* m_pHumanEditor;
	QScrollBar* m_pScrollBar;
};

#endif /* SRC_GUI_QFILEVIEW_H_ */
