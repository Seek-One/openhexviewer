/*
 * QFileViewController.cpp
 *
 *  Created on: 18 juil. 2018
 *      Author: ebeuque
 */

#include <math.h>
#include <algorithm>

#include <QTextEdit>
#include <QTextCursor>
#include <QPlainTextEdit>
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
#include <QKeyEvent>
#include <QByteArray>
#include <QMessageBox>
#include <QDebug>
#include <QByteArray>

#include "Global/QtCompat.h"
#include "GUI/QFileView.h"

#include "QFileViewController.h"

#define MAX_NB_FIND 1028

QFileViewController::QFileViewController(QFileView* pFileView)
{
	m_pFileView = pFileView;
	m_iFileSize = 0;
	m_iFilePos = 0;
	m_bIsFileOpen = false;

	m_iBytePerLine = 0;
	m_iVisibleRowCount = 0;
	m_iTotalRowCount = 0;

    m_nullFormat.setForeground(QColor("#f92671"));
    m_nullFormat.setFontWeight(QFont::Bold);

    m_offsetFormat.setForeground(QColor("#fa8400"));
    m_offsetFormat.setFontWeight(QFont::Bold);

    m_printableFormat.setForeground(QColor("#57bee1"));
    m_printableFormat.setFontWeight(QFont::Bold);

    m_whitespaceFormat.setForeground(QColor("#a6e22e"));
    m_whitespaceFormat.setFontWeight(QFont::Bold);

    m_otherAsciiFormat.setForeground(QColor("#4e9a06"));
    m_otherAsciiFormat.setFontWeight(QFont::Bold);

    m_nonAsciiFormat.setForeground(QColor("#c4a000"));
    m_nonAsciiFormat.setFontWeight(QFont::Bold);


	m_bHighLight = false;

	m_pModifications = new QEditorModificationList();

	connect(m_pFileView, SIGNAL(sizeChanged()), this, SLOT(updateView()));
	connect(m_pFileView, SIGNAL(rowChanged(int)), this, SLOT(moveToRow(int)));
	connect(m_pFileView, SIGNAL(textChangedHex(QPlainTextEdit*)), this, SLOT(handleTextChangedHex(QPlainTextEdit*)));
	connect(m_pFileView, SIGNAL(textChangedHuman(QPlainTextEdit*)), this, SLOT(handleTextChangedHuman(QPlainTextEdit*)));
	connect(m_pFileView, SIGNAL(selectionChangedHex(QPlainTextEdit*, QPlainTextEdit*)), this, SLOT(handleSelectionChangedHex(QPlainTextEdit*, QPlainTextEdit*)));
	connect(m_pFileView, SIGNAL(selectionChangedHuman(QPlainTextEdit*, QPlainTextEdit*)), this, SLOT(handleSelectionChangedHuman(QPlainTextEdit*, QPlainTextEdit*)));
	connect(m_pFileView, SIGNAL(cursorChangedHex(QPlainTextEdit*, QPlainTextEdit*)), this, SLOT(handleCursorChangedHex(QPlainTextEdit*, QPlainTextEdit*)));
	connect(m_pFileView, SIGNAL(cursorChangedHuman(QPlainTextEdit*, QPlainTextEdit*)), this, SLOT(handleCursorChangedHuman(QPlainTextEdit*, QPlainTextEdit*)));
	// connect(m_pFileView, SIGNAL(addNewByteHex(QPlainTextEdit*)), this, SLOT(addNewByteHex(QPlainTextEdit*)));
	// connect(m_pFileView, SIGNAL(removeByteHex(QPlainTextEdit*)), this, SLOT(removeByteHex(QPlainTextEdit*)));
	// connect(m_pFileView, SIGNAL(addNewByteHuman(QPlainTextEdit*, QString)), this, SLOT(addNewByteHuman(QPlainTextEdit*, QString)));
	// connect(m_pFileView, SIGNAL(removeByteHuman(QPlainTextEdit*)), this, SLOT(removeByteHuman(QPlainTextEdit*)));
}

QFileViewController::~QFileViewController()
{

}

bool QFileViewController::openFile(const QString& szFilePath)
{
	bool bRes = true;

	m_pModifications->clearModifications();

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

		emit fileOpened();
	}else{
		qWarning("[File] Unable to open file: %s", qPrintable(szFilePath));
	}

	return bRes;
}

bool QFileViewController::saveFile()
{
	if(!m_bIsFileOpen) {
		qWarning("[File] No file open");
		QMessageBox msgBox;
		msgBox.setText(tr("No file loaded"));
		msgBox.exec();
		return false;
	} 
	QFile file(m_file.fileName());
	//OPEN
	if (!file.open(QIODevice::ReadWrite)) {
		qWarning("[File] Failed to open file for reading and writing");
		return false;
	}
	
	char* pBuffer = new char[m_iBytePerLine];
	int iNbRead;

	for (int i = 0; i < m_iTotalRowCount; i++) {
		//SEEK
		if (!file.seek(i * m_iBytePerLine)) {
			qWarning("[File] Failed to seek to offset");
			if(pBuffer){
				delete[] pBuffer;
				pBuffer = NULL;
			}
			file.close();
			return false;
		}
		//READ
		iNbRead = file.read(pBuffer, m_iBytePerLine);
		if (iNbRead == 0) {
			break;
		}

		//MODIFY
		for (int j = 0; j < iNbRead; j++) {
			if (m_pModifications->existsPosition(i * m_iBytePerLine + j + m_iFilePos)) {
				pBuffer[j] = (m_pModifications->lastModificationAtPosition(i * m_iBytePerLine + j + m_iFilePos).data).at(0);
			}
		}

		//SEEK
		if (!file.seek(i * m_iBytePerLine)) {
			qWarning("[File] Failed to seek to offset");
			if(pBuffer){
				delete[] pBuffer;
				pBuffer = NULL;
			}
			file.close();
			return false;
		}
		//WRITE
		qint64 iBytesWritten = file.write(pBuffer);
		if (iBytesWritten == -1) {
			qWarning("[File] Failed to write data");
			if (pBuffer) {
				delete[] pBuffer;
				pBuffer = NULL;
			}
			file.close();
			return false;
		}
	}
	file.close();
	m_pModifications->clearModifications();
	readFile(m_iFilePos);
	return true;
}

bool QFileViewController::readFile(qint64 iStartOffset)
{
	QSignalBlocker blocker(m_pFileView);

	if(!m_bIsFileOpen) {
		return false;
	} 

	bool bRes;
	m_iFilePos = iStartOffset;
	bRes = m_file.seek(m_iFilePos);

	int iBufferSize = m_iBytePerLine;
	char* pBuffer = new char[m_iBytePerLine];

	quint32 iOffset;
	qint64 iNbRead = 1;
	QString szOffsetText;
	QString szHexText;
	QString szHumanText;
	char c;

	m_pFileView->setHexText("");
	m_pFileView->setHumanText("");
	
	QPlainTextEdit* pHexEditor = m_pFileView->getHexEditor();
	QPlainTextEdit* pHumanEditor = m_pFileView->getHumanEditor();
	
	QTextCursor hexCursor = pHexEditor->textCursor();
	QTextCursor humanCursor = pHumanEditor->textCursor();
	
	QTextCharFormat currentFormat;
	QTextCharFormat previousFormat;

	QString szTmp;

	int iPos = 0;

	if(bRes){
		iNbRead = m_file.read(pBuffer, iBufferSize);
		while (iPos < m_iVisibleRowCount * m_iBytePerLine && iNbRead > 0) {
			for (int i = 0; i < iNbRead; i++) {
				if (iPos % m_iBytePerLine == 0) {
					iOffset = (quint32)(m_iFilePos + iPos);
					QStringASPrintf(szTmp, "0x%08X", iOffset);
					szOffsetText += szTmp;
					szOffsetText += "\n";
					if (iPos > 0) {
						szHexText += "\n";
						szHumanText += "\n";	
					}
				}
			
				if (m_pModifications->existsPosition(m_iFilePos + iPos)) {
					c = (m_pModifications->lastModificationAtPosition(m_iFilePos + iPos).data).at(0);
				} else {
					c = pBuffer[i];
				}
					
				//Set hex Text
				QStringASPrintf(szTmp, "%02X", (unsigned char)c);
				QChar szChar(c);
				if (m_bHighLight) {
					if (szChar == QChar::Null) {
						currentFormat = m_nullFormat;
					} else if (c > 0x20 && c <= 0x7E) {
						currentFormat = m_printableFormat;
					} else if (szChar.isSpace()) {
						currentFormat = m_whitespaceFormat;
					} else if (szChar.unicode() < 128) {
						currentFormat = m_otherAsciiFormat;
					} else {
						currentFormat = m_nonAsciiFormat;
					}
					
					if ((iPos != 0 && currentFormat != previousFormat) || iPos >= m_iVisibleRowCount * m_iBytePerLine - 1) {
						hexCursor.insertText(szHexText, previousFormat);
						humanCursor.insertText(szHumanText, previousFormat);
						szHexText = "";
						szHumanText = "";
					}
					previousFormat = currentFormat;
				}
				szHexText += szTmp;
				if (iPos % m_iBytePerLine < iNbRead - 1) {
					szHexText += " ";
				}
				if (szChar == QChar::Null) {
					szHumanText += "0"; 
				} else if (c > 0x20 && c <= 0x7E) {
					szHumanText += c;
				} else if (szChar.isSpace()) {
					szHumanText += "_";
				} else if (szChar.unicode() < 128) {
					szHumanText += "×";
				} else {
					szHumanText += "·";
				}
				
				++iPos;
			}
			iNbRead = m_file.read(pBuffer, iBufferSize);
		}
		if (!m_bHighLight) {
			QTextCharFormat normalFormat;
			previousFormat = normalFormat;
		} 
		hexCursor.insertText(szHexText, previousFormat);
		humanCursor.insertText(szHumanText, previousFormat);
		m_pFileView->setOffsetText(szOffsetText);
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
		emit fileClosed();
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
	if(iFirstVisibleRow < iRowStart){
		iPosStart += (iRowStart-iFirstVisibleRow) * m_iBytePerLine;
	}

	int iNbSelectedLine = (iRowEnd-iRowStart);
	iNbSelectedLine++;

	m_pFileView->moveToRow(iFirstVisibleRow);
	m_pFileView->selectText(iPosStart, iPosStart + (int)(iSize), (iRowStart-iFirstVisibleRow), iNbSelectedLine);
}

void QFileViewController::getSelectionOffset(qint64& offset, qint64& size)
{
	QPlainTextEdit* pEditor = m_pFileView->getHumanEditor();
	QTextCursor cursorEditor = pEditor->textCursor();
	qint64 iSelectionStart = cursorEditor.selectionStart();
	qint64 iSelectionEnd = cursorEditor.selectionEnd();
	qint64 iSelectionSize = abs(iSelectionEnd - iSelectionStart);
	QString szTextEditor = pEditor->toPlainText();
	offset = cursorEditor.selectionStart() - szTextEditor.mid(0, iSelectionStart).count("\n") + m_iFilePos;
	size = abs(iSelectionSize - szTextEditor.mid(iSelectionStart, iSelectionSize).count("\n"));
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
	QSignalBlocker block(m_pFileView);

	updateDisplayData();
	readFile(m_iFilePos);
}

void QFileViewController::moveToRow(int iRow)
{
	QSignalBlocker block(m_pFileView);

	iRow = qMin(iRow, m_iTotalRowCount - m_iVisibleRowCount);

	qint64 iOffset = (((qint64)iRow) * m_iBytePerLine);
	readFile(iOffset);
}

void QFileViewController::handleTextChangedHex(QPlainTextEdit* pHexEditor) 
{
	QSignalBlocker block(m_pFileView);
	
	QTextCursor tHexCursor = pHexEditor->textCursor();
	int iSelectionStart = tHexCursor.selectionStart();
	QString szHexText = pHexEditor->toPlainText();

	QString szTmp;
	if (szHexText.mid(tHexCursor.selectionStart() - 1, 1).contains(" ") || szHexText.mid(tHexCursor.selectionStart() - 1, 1).contains("\n")) { // AB |A|A
		szTmp = szHexText.mid(tHexCursor.selectionStart(), 2);
		++iSelectionStart;
	} else { // B|A| AA
		szTmp = szHexText.mid(tHexCursor.selectionStart() - 2, 2);
	}

	bool bOk;
	int iText = szTmp.toInt(&bOk, 16);
	if (bOk) {
		char cRes = static_cast<char>(iText);
		QByteArray qbytechar(&cRes, 1);
		m_pModifications->addModification(m_iFilePos + tHexCursor.position() / 3, Action::Update, 1, qbytechar);
	}
	readFile(m_iFilePos);

	while (iSelectionStart < szHexText.length() && (szHexText.mid(iSelectionStart, 1).contains(" ") || szHexText.mid(iSelectionStart, 1).contains("\n"))) {
		++iSelectionStart;
	}
	tHexCursor.setPosition(iSelectionStart);
	tHexCursor.setPosition(iSelectionStart + 1, QTextCursor::KeepAnchor);
	
	pHexEditor->setTextCursor(tHexCursor);
}	

void QFileViewController::handleTextChangedHuman(QPlainTextEdit* pHumanEditor) 
{
	QSignalBlocker blocker(m_pFileView);

	QTextCursor tHumanCursor = pHumanEditor->textCursor();
	int iSelectionStart = tHumanCursor.selectionStart();
	int iNbEnter = pHumanEditor->toPlainText().mid(0, tHumanCursor.selectionStart()).count("\n");
	
	char cRes = pHumanEditor->toPlainText().mid(tHumanCursor.position() - 1, 1).at(0).toLatin1();

	QByteArray qbytechar(&cRes);
	m_pModifications->addModification(m_iFilePos + tHumanCursor.selectionStart() - 1 - iNbEnter + m_iFilePos, Action::Update, 1, qbytechar);

	readFile(m_iFilePos);

	tHumanCursor.setPosition(iSelectionStart);
	tHumanCursor.setPosition(iSelectionStart + 1, QTextCursor::KeepAnchor);
	pHumanEditor->setTextCursor(tHumanCursor);
}

void QFileViewController::handleSelectionChangedHex(QPlainTextEdit* pHexEditor, QPlainTextEdit* pHumanEditor) 
{
	QSignalBlocker blocker(m_pFileView);

	int iSelectionStart;
	int iSelectionEnd;
	
	QTextCursor tHexCursor = pHexEditor->textCursor();
	if (tHexCursor.position() == tHexCursor.selectionEnd()) {
		iSelectionStart = tHexCursor.selectionStart();
		iSelectionEnd = tHexCursor.selectionEnd();
	} else {
		iSelectionStart = tHexCursor.selectionEnd();
		iSelectionEnd = tHexCursor.selectionStart();
	}
	QString szHexText = pHexEditor->toPlainText();

	int iNbEnterStart = szHexText.mid(0, iSelectionStart).count("\n");
	int iNbEnterEnd = szHexText.mid(iSelectionStart, abs(iSelectionEnd - iSelectionStart)).count("\n");

	if (abs(iSelectionEnd - iSelectionStart) != 1) {
		//START
		while (iSelectionStart > 0 && iSelectionStart < szHexText.length() && szHexText.mid(iSelectionStart - 1 * (iSelectionStart < iSelectionEnd), 1) != " " && szHexText.mid(iSelectionStart - 1 * (iSelectionStart < iSelectionEnd), 1) != "\n") {
			if (iSelectionStart < iSelectionEnd) {
				iSelectionStart--;
			} else {
				iSelectionStart++;
			}
		}
		tHexCursor.setPosition(iSelectionStart);
		
		//END
		while (iSelectionEnd < szHexText.length() && iSelectionEnd > 0 && szHexText.mid(iSelectionEnd - 1 * (iSelectionEnd < iSelectionStart), 1) != " " && szHexText.mid(iSelectionEnd - 1 * (iSelectionEnd < iSelectionStart), 1) != "\n") {
			if (iSelectionEnd > iSelectionStart) {
				iSelectionEnd++;
			} else {
				iSelectionEnd--;
			}
		}
		tHexCursor.setPosition(iSelectionEnd, QTextCursor::KeepAnchor);
		pHexEditor->setTextCursor(tHexCursor);
	}
	QTextCursor c = pHumanEditor->textCursor();
	if (iSelectionStart < iSelectionEnd) {
		c.setPosition(iSelectionStart / 3 + iNbEnterStart);
		c.setPosition(iSelectionEnd / 3 + iNbEnterEnd + iNbEnterStart + 1, QTextCursor::KeepAnchor);
	} else {
		c.setPosition(iSelectionEnd / 3 + iNbEnterStart);
		c.setPosition(iSelectionStart / 3 + iNbEnterEnd + iNbEnterStart + 1, QTextCursor::KeepAnchor);
	}
	pHumanEditor->setTextCursor(c);	

	emit onBytesSelectionChanged(c.selectionStart() - iNbEnterStart + m_iFilePos, c.selectionEnd() - c.selectionStart() - iNbEnterEnd);

	emit onBytesChanged(szHexText.mid(tHexCursor.selectionStart(), abs(tHexCursor.selectionEnd() - tHexCursor.selectionStart())));
}

void QFileViewController::handleSelectionChangedHuman(QPlainTextEdit* pHumanEditor, QPlainTextEdit* pHexEditor) 
{
	QSignalBlocker blocker(m_pFileView);

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
	QSignalBlocker blocker(m_pFileView);
}

void QFileViewController::handleCursorChangedHuman(QPlainTextEdit* pHumanEditor, QPlainTextEdit* pHexEditor) 
{
	QSignalBlocker blocker(m_pFileView);
}

// void QFileViewController::addNewByteHex(QPlainTextEdit* pHexEditor)
// {
// 	QSignalBlocker block(m_pFileView);
	
// 	QTextCursor tHexCursor = pHexEditor->textCursor();
// 	int iSelectionStart = tHexCursor.selectionStart();
// 	QString szHexText = pHexEditor->toPlainText();

// 	QString szTmp = "00";

// 	bool bOk;
// 	int iText = szTmp.toInt(&bOk, 16);
// 	if (bOk) {
// 		char cRes = static_cast<char>(iText);
// 		QByteArray qbytechar(&cRes);
// 		m_pModifications->addModification(tHexCursor.position() / 3 + m_iFilePos, Action::Add, 1, qbytechar);
// 	}
// 	readFile(m_iFilePos);

// 	tHexCursor.setPosition(iSelectionStart);
// 	tHexCursor.setPosition(iSelectionStart + 1, QTextCursor::KeepAnchor);
// 	pHexEditor->setTextCursor(tHexCursor);
// }

// void QFileViewController::removeByteHex(QPlainTextEdit* pHexEditor)
// {
// 	QSignalBlocker block(m_pFileView);
	
// 	QTextCursor tHexCursor = pHexEditor->textCursor();
// 	int iSelectionStart = tHexCursor.selectionStart();
// 	QString szHexText = pHexEditor->toPlainText();


// 	if (m_pModifications->existsPosition(tHexCursor.position() / 3 + m_iFilePos)) {
// 		m_pModifications->addModification(tHexCursor.position() / 3 + m_iFilePos, Action::Remove, 1, ""); //m_pModifications->lastModificationAtPosition(tHexCursor.position() / 3 + m_iFilePos).data);
// 	} else {
// 		m_pModifications->addModification(tHexCursor.position() / 3 + m_iFilePos, Action::Remove, 1, "");
// 	}

// 	readFile(m_iFilePos);

// 	tHexCursor.setPosition(iSelectionStart);
// 	tHexCursor.setPosition(iSelectionStart + 1, QTextCursor::KeepAnchor);
// 	pHexEditor->setTextCursor(tHexCursor);
// }

// void QFileViewController::addNewByteHuman(QPlainTextEdit* pHumanEditor, QString szText)
// {
// 	QSignalBlocker blocker(m_pFileView);

// 	QTextCursor tHumanCursor = pHumanEditor->textCursor();
// 	int iSelectionStart = tHumanCursor.selectionStart();
// 	int iNbEnter = pHumanEditor->toPlainText().mid(0, tHumanCursor.selectionStart()).count("\n");
	
// 	m_pModifications->addModification(tHumanCursor.selectionStart() - iNbEnter + m_iFilePos, Action::Add, 1, szText.toUtf8());

// 	readFile(m_iFilePos);

// 	tHumanCursor.setPosition(iSelectionStart + 1);
// 	pHumanEditor->setTextCursor(tHumanCursor);
// }

// void QFileViewController::removeByteHuman(QPlainTextEdit* pHumanEditor)
// {
// 	QSignalBlocker blocker(m_pFileView);

// 	QTextCursor tHumanCursor = pHumanEditor->textCursor();
// 	int iSelectionStart = tHumanCursor.selectionStart();
// 	int iNbEnter = pHumanEditor->toPlainText().mid(0, tHumanCursor.selectionStart()).count("\n");
	
// 	REMOVE
	
// 	readFile(m_iFilePos);

// 	tHumanCursor.setPosition(iSelectionStart + 1);
// 	pHumanEditor->setTextCursor(tHumanCursor);
// }

void QFileViewController::findAllOccurrencesRegex(const QByteArray &byteArray, QList<qint64>* plstPositions)
{
	plstPositions->clear();
	int iLengthSubString = byteArray.length();
	QRegularExpression re(QRegularExpression::escape(byteArray));
	char* pBuffer = new char[iLengthSubString];
	bool bFound;
	int iNbRead;
	if (!m_bIsFileOpen) {
		qWarning("[File] No file open");
		QMessageBox msgBox;
		msgBox.setText(tr("No file loaded"));
		msgBox.exec();
		return;
	}
	QFile file(m_file.fileName());
	//OPEN
	if (!file.open(QIODevice::ReadOnly)) {
		qWarning("[File] Failed to open file for reading");
		return;
	}

	for (int i = 0; i < m_iFileSize - iLengthSubString; i++) {
		bFound = true;
		//SEEK
		if (!file.seek(i)) {
			qWarning("[File] Failed to seek to offset");
			if(pBuffer){
				delete[] pBuffer;
				pBuffer = NULL;
			}
			file.close();
			return;
		}
		//READ
		iNbRead = file.read(pBuffer, iLengthSubString);
		if (iNbRead != iLengthSubString) {
			break;
		}
		for (int j = 0; j < iNbRead; j++) {
			if (m_pModifications->existsPosition(i + j)) {
				pBuffer[j] = m_pModifications->lastModificationAtPosition(i + j).data.data()[0];
			}
			if (pBuffer[j] != byteArray.at(j)) {
				bFound = false;
				break;
			}
		}
		if (bFound) {
			plstPositions->append(i);
			i += iLengthSubString - 1;
			if (plstPositions->size() >= MAX_NB_FIND) {
				break;
			}
		}
	}
	file.close();

	if (plstPositions->size() > 0) {
		selectFileData(plstPositions->at(0), byteArray.length()); //enter problem
	} else {
		QMessageBox msgBox;
		msgBox.setText(tr("No match found"));
		msgBox.exec();
	}
}

void QFileViewController::colorText(bool bIsChecked)
{
	m_bHighLight = bIsChecked;
	if(m_bIsFileOpen) {
		readFile(m_iFilePos);
	} 
}

void QFileViewController::selection(QString& szText)
{
	
	QPlainTextEdit* editor = m_pFileView->getHumanEditor();
	QTextCursor cursor = editor->textCursor();

	char* pBuffer = new char[m_iBytePerLine];
	int iNbRead;
	if (!m_bIsFileOpen) {
		qWarning("[File] No file open");
		QMessageBox msgBox;
		msgBox.setText(tr("No file loaded"));
		msgBox.exec();
		return;
	}
	QFile file(m_file.fileName());
	if (!file.open(QIODevice::ReadOnly)) {
		qWarning("[File] Failed to open file for reading");
		return;
	}
	qint64 iSelectionStart = cursor.selectionStart();
	qint64 iSelectionEnd = cursor.selectionEnd() - editor->toPlainText().mid(iSelectionStart, abs(cursor.selectionEnd() - iSelectionStart)).count("\n");
	for (int i = iSelectionStart; i < iSelectionEnd; i += m_iBytePerLine) {
		//SEEK
		if (!file.seek(m_iFilePos + i)) {
			qWarning("[File] Failed to seek to offset");
			if(pBuffer){
				delete[] pBuffer;
				pBuffer = NULL;
			}
			file.close();
			return;
		}
		//READ
		qint64 minNbRead = std::min((qint64)m_iBytePerLine, iSelectionEnd - i);
		iNbRead = file.read(pBuffer, minNbRead);
		if (iNbRead == 0) {
			break;
		}
		for (qint64 i = 0; i < minNbRead; i++) {
			if (m_pModifications->existsPosition(m_iFilePos + i)) {
				szText += (m_pModifications->lastModificationAtPosition(m_iFilePos + i).data).at(0);
			} else {
				szText += pBuffer[i];
			}
		}
	}
	if(pBuffer){
		delete[] pBuffer;
		pBuffer = NULL;
	}

	file.close();
}