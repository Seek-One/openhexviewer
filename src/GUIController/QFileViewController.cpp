/*
 * QFileViewController.cpp
 *
 *  Created on: 18 juil. 2018
 *      Author: ebeuque
 */

#include <math.h>
#include <algorithm>

#include <QTextCursor>
#include <QPlainTextEdit>
#include <QKeyEvent>

#include "Global/QtCompat.h"
#include "GUI/QFileView.h"

#include "QFileViewController.h"

QFileViewController::QFileViewController(QFileView* pFileView)
{
	m_pFileView = pFileView;
	m_iFileSize = 0;
	m_iFilePos = 0;
	m_bIsFileOpen = false;

	m_iBytePerLine = 0;
	m_iVisibleRowCount = 0;
	m_iTotalRowCount = 0;

	connect(m_pFileView, SIGNAL(sizeChanged()), this, SLOT(updateView()));
	connect(m_pFileView, SIGNAL(rowChanged(int)), this, SLOT(moveToRow(int)));
	connect(m_pFileView, SIGNAL(textChangedHex(QPlainTextEdit*, QPlainTextEdit*)), this, SLOT(handleTextChangedHex(QPlainTextEdit*, QPlainTextEdit*)));
	connect(m_pFileView, SIGNAL(textChangedHuman(QPlainTextEdit*, QPlainTextEdit*)), this, SLOT(handleTextChangedHuman(QPlainTextEdit*, QPlainTextEdit*)));
	connect(m_pFileView, SIGNAL(selectionChangedHex(QPlainTextEdit*, QPlainTextEdit*)), this, SLOT(handleSelectionChangedHex(QPlainTextEdit*, QPlainTextEdit*)));
	connect(m_pFileView, SIGNAL(selectionChangedHuman(QPlainTextEdit*, QPlainTextEdit*)), this, SLOT(handleSelectionChangedHuman(QPlainTextEdit*, QPlainTextEdit*)));
	connect(m_pFileView, SIGNAL(cursorChangedHex(QPlainTextEdit*, QPlainTextEdit*)), this, SLOT(handleCursorChangedHex(QPlainTextEdit*, QPlainTextEdit*)));
	connect(m_pFileView, SIGNAL(cursorChangedHuman(QPlainTextEdit*, QPlainTextEdit*)), this, SLOT(handleCursorChangedHuman(QPlainTextEdit*, QPlainTextEdit*)));
}

QFileViewController::~QFileViewController()
{

}

bool QFileViewController::openFile(const QString& szFilePath)
{
	bool bRes = true;

	if(m_bIsFileOpen){
		closeFile();
	}

	m_file.setFileName(szFilePath);
	bRes = m_file.open(QIODevice::ReadOnly);
	if(bRes){
		m_bIsFileOpen = true;
		m_iFileSize = m_file.size();

		updateDisplayData();

		bRes = readFile(0);
	}else{
		qWarning("[File] Unable to open file: %s", qPrintable(szFilePath));
	}

	return bRes;
}

bool QFileViewController::readFile(qint64 iStartOffset)
{
	QSignalBlocker blocker(m_pFileView);

	bool bRes;
	m_iFilePos = iStartOffset;
	bRes = m_file.seek(m_iFilePos);

	int iBufferSize = m_iBytePerLine;
	char* pBuffer = new char[m_iBytePerLine];

	quint32 iOffset;
	qint64 iNbRead;
	QString szOffsetText;
	QString szHexText;
	QString szHumanText;
	char c;

	QString szTmp;

	if(bRes){
		for(int i=0; i<m_iVisibleRowCount; i++){
			iNbRead = m_file.read(pBuffer, iBufferSize);
			if(iNbRead > 0){

				// Prepend a line break if not first row
				if(i>0){
					szOffsetText += "\n";
					szHexText += "\n";
					szHumanText += "\n";
				}

				iOffset = (quint32)(m_iFilePos+i*m_iBytePerLine);
				QStringASPrintf(szTmp, "0x%08X", iOffset);
				szOffsetText += szTmp;

				for(int j=0; j<iNbRead; j++)
				{
					// Set hex text
					c = pBuffer[j];
					QStringASPrintf(szTmp, "%02X", (unsigned char)c);
					szHexText += szTmp;
					if (j < iNbRead - 1) {
						szHexText +=" ";
						
					}

					// Set human text
					if(c >= 0x20 && c <= 0x7E){
						szHumanText += c;
					}else{
						szHumanText += ".";
					}
				}
			}
		}

		m_pFileView->setOffsetText(szOffsetText);
		m_pFileView->setHexText(szHexText);
		m_pFileView->setHumanText(szHumanText);
	}

	if(pBuffer){
		delete[] pBuffer;
		pBuffer = NULL;
	}

	return bRes;
}

void QFileViewController::closeFile()
{
	if(m_bIsFileOpen){
		m_file.close();
		m_bIsFileOpen = false;
	}
}

void QFileViewController::selectFileData(qint64 offset, qint64 size)
{
	int iRowStart = offset / m_iBytePerLine;
	int iPosStart = offset % m_iBytePerLine;

	qint64 offsetEnd = offset+size-1;
	int iRowEnd = offsetEnd / m_iBytePerLine;
	iRowEnd = qMin(iRowEnd, iRowStart+m_iVisibleRowCount-1);
	//int iColEnd = offsetEnd % m_iBytePerLine;

	int iFirstVisibleRow = std::min(iRowStart, std::max(m_iTotalRowCount-m_iVisibleRowCount, 0));
	qint64 iPosMax = m_iVisibleRowCount*m_iBytePerLine - iPosStart;
	qint64 iSize = std::min(size, iPosMax);
	qDebug("%lld,%lld", iSize, iPosMax);
	if(iFirstVisibleRow < iRowStart){
		iPosStart += (iRowStart-iFirstVisibleRow) * m_iBytePerLine;
	}

	int iNbSelectedLine = (iRowEnd-iRowStart);
	iNbSelectedLine++;

	m_pFileView->moveToRow(iFirstVisibleRow);
	m_pFileView->selectText(iPosStart, iPosStart + (int)(iSize), (iRowStart-iFirstVisibleRow), iNbSelectedLine);
}

void QFileViewController::updateDisplayData()
{
	m_iBytePerLine = m_pFileView->getBytePerLine();
	m_iTotalRowCount = ceil(m_iFileSize / (float)m_iBytePerLine);
	m_iVisibleRowCount = m_pFileView->getVisibleRowCount();
	m_pFileView->setRowCount(m_iTotalRowCount);
}

void QFileViewController::updateView()
{
	if(m_bIsFileOpen){
		updateDisplayData();
		readFile(m_iFilePos);
	}
}

void QFileViewController::moveToRow(int iRow)
{
	iRow = qMin(iRow, m_iTotalRowCount - m_iVisibleRowCount);

	qint64 iOffset = (((qint64)iRow) * m_iBytePerLine);
	readFile(iOffset);
}

void QFileViewController::handleTextChangedHex(QPlainTextEdit* pHexEditor, QPlainTextEdit* pHumanEditor) 
{
	QSignalBlocker blocker(pHumanEditor);
	
	QTextCursor tHexCursor = pHexEditor->textCursor();
	QTextCursor tHumanCursor = pHumanEditor->textCursor();
	QString szHexText = pHexEditor->toPlainText();

	int iNbEnter = szHexText.mid(0, tHexCursor.selectionStart()).count("\n");

	QString szTmp;
	int iGap = 0;
	if (szHexText.mid(tHexCursor.selectionStart() - 1, 1) == " " || szHexText.mid(tHexCursor.selectionStart() - 1, 1) == "\n") { // AA |A|A
		szTmp = szHexText.mid(tHexCursor.selectionStart() - 3, 2);
		iGap = 1;

	} else { // A|A| AA
		szTmp = szHexText.mid(tHexCursor.selectionStart() - 1, 2);
	}
	tHumanCursor.setPosition(tHexCursor.position() / 3 + iNbEnter - iGap);
	tHumanCursor.setPosition(tHexCursor.position() / 3 + 1 + iNbEnter - iGap, QTextCursor::KeepAnchor);
	
	bool bOk;
	int iText = szTmp.toInt(&bOk, 16);
	if (bOk) {
		char cRes = static_cast<char>(iText);
		if (cRes >= 0x20 && cRes <= 0x7E) {
			tHumanCursor.insertText(QChar(cRes));
		} else {
			tHumanCursor.insertText(".");
		}
	}
	handleCursorChangedHuman(pHumanEditor, pHexEditor);

}	

void QFileViewController::handleTextChangedHuman(QPlainTextEdit* pHumanEditor, QPlainTextEdit* pHexEditor) 
{
	QSignalBlocker blocker(pHexEditor);

	QTextCursor humanCursor = pHumanEditor->textCursor();
	int iEnter = pHumanEditor->toPlainText().mid(0, humanCursor.selectionStart()).count("\n");

	QTextCursor cursorEditor = pHexEditor->textCursor();
	cursorEditor.setPosition((humanCursor.position() - 1 - iEnter) * 3);
	cursorEditor.setPosition((humanCursor.position() - 1 - iEnter) * 3 + 2, QTextCursor::KeepAnchor);
	QString szTmp;
	QStringASPrintf(szTmp, "%02X", (unsigned char)pHumanEditor->toPlainText().mid(humanCursor.position() - 1, 1).at(0).toLatin1());
	cursorEditor.insertText(szTmp);
}

void QFileViewController::handleSelectionChangedHex(QPlainTextEdit* pHexEditor, QPlainTextEdit* pHumanEditor) 
{
	QSignalBlocker blocker(pHumanEditor);

	QTextCursor tHexCursor = pHexEditor->textCursor();

	int iSelectionStart = tHexCursor.selectionStart();
	int iSelectionEnd = tHexCursor.selectionEnd();

	int iNbEnterStart = pHexEditor->toPlainText().mid(0, iSelectionStart).count("\n");
	int iNbEnterEnd = pHexEditor->toPlainText().mid(iSelectionStart, abs(iSelectionEnd - iSelectionStart)).count("\n");

	QTextCursor c = pHumanEditor->textCursor();
	c.setPosition(iSelectionStart / 3 + iNbEnterStart);
	c.setPosition(iSelectionEnd / 3 + iNbEnterEnd + iNbEnterStart + 1, QTextCursor::KeepAnchor);
	pHumanEditor->setTextCursor(c);

	emit onBytesSelectionChanged(c.selectionStart() - iNbEnterStart + m_iFilePos, c.selectionEnd() - c.selectionStart() - iNbEnterEnd);

	emit onBytesChanged(pHexEditor->toPlainText().mid(tHexCursor.selectionStart(), abs(tHexCursor.selectionEnd() - tHexCursor.selectionStart())));
}

void QFileViewController::handleSelectionChangedHuman(QPlainTextEdit* pHumanEditor, QPlainTextEdit* pHexEditor) 
{
	QSignalBlocker blocker(pHexEditor);

	QTextCursor tHumanCursor = pHumanEditor->textCursor();
	QTextCursor tHexCursor = pHexEditor->textCursor();

	int iSelectionStart = tHumanCursor.selectionStart();
	int iSelectionEnd = tHumanCursor.selectionEnd();

	int iNbEnterStart = pHumanEditor->toPlainText().mid(0, iSelectionStart).count("\n");
	int iNbEnterEnd = pHumanEditor->toPlainText().mid(iSelectionStart, abs(iSelectionEnd - iSelectionStart)).count("\n");

	QTextCursor c = pHexEditor->textCursor();
	c.setPosition((iSelectionStart - iNbEnterStart) * 3);
	c.setPosition((iSelectionEnd - iNbEnterEnd - iNbEnterStart) * 3 - 1 * (iSelectionStart - iSelectionEnd == 0 ? -2 : 1), QTextCursor::KeepAnchor);  // -1 removes last space
	pHexEditor->setTextCursor(c);
	
	emit onBytesSelectionChanged(iSelectionStart - iNbEnterStart + m_iFilePos, iSelectionEnd - iSelectionStart - iNbEnterEnd);

	emit onBytesChanged(pHexEditor->toPlainText().mid(tHexCursor.selectionStart(), abs(tHexCursor.selectionEnd() - tHexCursor.selectionStart())));
}

void QFileViewController::handleCursorChangedHex(QPlainTextEdit* pHexEditor, QPlainTextEdit* pHumanEditor) 
{
	QSignalBlocker blocker(pHumanEditor);
}

void QFileViewController::handleCursorChangedHuman(QPlainTextEdit* pHumanEditor, QPlainTextEdit* pHexEditor) 
{
	QSignalBlocker blocker(pHexEditor);
}