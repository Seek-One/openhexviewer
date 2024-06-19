/*
 * QFileViewController.h
 *
 *  Created on: 18 juil. 2018
 *      Author: ebeuque
 */

#ifndef SRC_GUICONTROLLER_QFILEVIEWCONTROLLER_H_
#define SRC_GUICONTROLLER_QFILEVIEWCONTROLLER_H_

#include <QString>
#include <QFile>

class QFileView;
class QPlainTextEdit;
class QKeyEvent;

class QFileViewController : public QObject
{
	Q_OBJECT
public:
	QFileViewController(QFileView* pFileView);
	virtual ~QFileViewController();

	bool openFile(const QString& szFilePath);
	bool saveFile();

	void closeFile();

	void selectFileData(qint64 offset, qint64 size);

signals:
	void onBytesSelectionChanged(qint64 offset, qint64 size); 
	void onBytesChanged(QString szHex);

public slots:
	void updateView();
	void moveToRow(int iRow);

	void handleTextChangedHex(QPlainTextEdit* pHexEditor, QPlainTextEdit* pHumanEditor);
	void handleTextChangedHuman(QPlainTextEdit* pHumanEditor, QPlainTextEdit* pHexEditor);
	void handleSelectionChangedHex(QPlainTextEdit* pHexEditor, QPlainTextEdit* pHumanEditor);
	void handleSelectionChangedHuman(QPlainTextEdit* pHumanEditor, QPlainTextEdit* pHexEditor);
	void handleCursorChangedHex(QPlainTextEdit* pHexEditor, QPlainTextEdit* pHumanEditor); 
	void handleCursorChangedHuman(QPlainTextEdit* pHumanEditor, QPlainTextEdit* pHexEditor); 
	
	void addNewByteHex(QPlainTextEdit* pHexEditor);
	void removeByteHex(QPlainTextEdit* m_pHexEditor);

private:
	QFileView* m_pFileView;

	//Store data
	QString m_szData;
	void updateText(QString szText, qint64 iStartOffset);


	// Current file
	bool m_bIsFileOpen;
	QFile m_file;
	qint64 m_iFileSize;
	qint64 m_iFilePos;

	// Current display
	int m_iBytePerLine;
	int m_iTotalRowCount;
	int m_iVisibleRowCount;

	void updateDisplayData();
	bool readFile(qint64 iStartOffset);

	bool keyPressHumanEditor(QKeyEvent* keyEvent);
	bool keyPressHexEditor(QKeyEvent* keyEvent);
	bool eventHexEditor(QObject *obj, QEvent *event); 
	bool eventHumanEditor(QObject *obj, QEvent *event); 

};

#endif /* SRC_GUICONTROLLER_QFILEVIEWCONTROLLER_H_ */
