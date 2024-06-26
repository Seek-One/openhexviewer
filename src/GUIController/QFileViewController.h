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
#include <QTextCharFormat>
#include "GUIModel/QEditorModificationList.h"

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

	void getSelectionOffset(qint64& offset, qint64& size);

signals:
	void onBytesSelectionChanged(qint64 offset, qint64 size); 
	void onBytesChanged(QString szHex);
	void fileOpened();
	void fileClosed();
 	void doChanges();

public slots:
	void updateView();
	void moveToRow(int iRow);
	void colorText(bool bIsChecked);
	void selection(QString& szText);

	void handleTextChangedHex(QPlainTextEdit* pHexEditor);
	void handleTextChangedHuman(QPlainTextEdit* pHumanEditor);
	void handleSelectionChangedHex(QPlainTextEdit* pHexEditor, QPlainTextEdit* pHumanEditor);
	void handleSelectionChangedHuman(QPlainTextEdit* pHumanEditor, QPlainTextEdit* pHexEditor);
	void handleCursorChangedHex(QPlainTextEdit* pHexEditor, QPlainTextEdit* pHumanEditor); 
	void handleCursorChangedHuman(QPlainTextEdit* pHumanEditor, QPlainTextEdit* pHexEditor); 
	
	// void addNewByteHex(QPlainTextEdit* pHexEditor);
	// void removeByteHex(QPlainTextEdit* m_pHexEditor);
	// void addNewByteHuman(QPlainTextEdit* m_pHumanEditor, QString szText);
	// void removeByteHuman(QPlainTextEdit* m_pHumanEditor);

	void findAllOccurrencesRegex(const QByteArray &byteArray, QList<qint64>* plstPositions);
private:
	QFileView* m_pFileView;

	// Current file
	bool m_bIsFileOpen;
	QFile m_file;
	qint64 m_iFileSize;
	qint64 m_iFilePos;

	// Current display
	int m_iBytePerLine;
	int m_iTotalRowCount;
	int m_iVisibleRowCount;

	bool m_bHighLight;

	//List of modifications of file
	QEditorModificationList* m_pModifications;

	void updateDisplayData();
	bool readFile(qint64 iStartOffset);

	bool keyPressHumanEditor(QKeyEvent* keyEvent);
	bool keyPressHexEditor(QKeyEvent* keyEvent);
	bool eventHexEditor(QObject *obj, QEvent *event); 
	bool eventHumanEditor(QObject *obj, QEvent *event); 

    QTextCharFormat m_nonAsciiFormat;
    QTextCharFormat m_otherAsciiFormat;
    QTextCharFormat m_whitespaceFormat;
    QTextCharFormat m_printableFormat;
	QTextCharFormat m_nullFormat;
    QTextCharFormat m_offsetFormat;
};

#endif /* SRC_GUICONTROLLER_QFILEVIEWCONTROLLER_H_ */
