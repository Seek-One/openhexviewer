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
	void selectText(int iPosStart, int iPosEnd, int iStartNbRow, int iNbRow);

	void selectTextRow(int iPosStart, int iPosEnd, int iRowStart, int iRowEnd, int iNbBytePerLine);
signals:
	void sizeChanged();
	void rowChanged(int iRow);

	void textChangedHex(QPlainTextEdit* pHexEditor, QPlainTextEdit* pHumanEditor);
	void textChangedHuman(QPlainTextEdit* pHumanEditor, QPlainTextEdit* pHexEditor);
	void selectionChangedHex(QPlainTextEdit* pHexEditor, QPlainTextEdit* pHumanEditor);
	void selectionChangedHuman(QPlainTextEdit* pHumanEditor, QPlainTextEdit* pHexEditor);
	void cursorChangedHex(QPlainTextEdit* pHexEditor, QPlainTextEdit* pHumanEditor);
	void cursorChangedHuman(QPlainTextEdit* pHumanEditor, QPlainTextEdit* pHexEditor);

protected:
	void resizeEvent(QResizeEvent *event);

	bool eventFilter(QObject *obj, QEvent *event) override;

private:
	QPlainTextEdit* m_pOffsetEditor;
	QPlainTextEdit* m_pHexEditor;
	QPlainTextEdit* m_pHumanEditor;
	QScrollBar* m_pScrollBar;
	
	bool eventHexEditor(QObject *obj, QEvent *event); 
	bool eventHumanEditor(QObject *obj, QEvent *event); 
};

#endif /* SRC_GUI_QFILEVIEW_H_ */
