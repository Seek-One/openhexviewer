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
class QPlainTextEdit;
class QScrollBar;

class QFileView : public QWidget
{
	Q_OBJECT
public:
	QFileView(QWidget* pParent = NULL);
	virtual ~QFileView();

	void setRowCount(int iMax);
	void setCurrentRow(int iRow);

	int getBytePerLine() const;
	int getVisibleRowCount() const;

	void setOffsetText(const QString& szText);
	void setHexText(const QString& szText);
	void setHumanText(const QString& szText);

	void moveToRow(int iRow);
	void selectText(int iPosStart, int iPosEnd, int iNbRow);

signals:
	void sizeChanged();
	void rowChanged(int iRow);

protected:
	void resizeEvent(QResizeEvent *event);

private:
	QPlainTextEdit* m_pOffsetEditor;
	QPlainTextEdit* m_pHexEditor;
	QPlainTextEdit* m_pHumanEditor;
	QScrollBar* m_pScrollBar;
};

#endif /* SRC_GUI_QFILEVIEW_H_ */
