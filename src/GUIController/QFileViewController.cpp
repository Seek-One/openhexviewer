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
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
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
	connect(m_pFileView, SIGNAL(addNewByteHex(QPlainTextEdit*)), this, SLOT(addNewByteHex(QPlainTextEdit*)));
	connect(m_pFileView, SIGNAL(removeByteHex(QPlainTextEdit*)), this, SLOT(removeByteHex(QPlainTextEdit*)));
	connect(m_pFileView, SIGNAL(addNewByteHuman(QPlainTextEdit*, QString)), this, SLOT(addNewByteHuman(QPlainTextEdit*, QString)));
	connect(m_pFileView, SIGNAL(removeByteHuman(QPlainTextEdit*)), this, SLOT(removeByteHuman(QPlainTextEdit*)));
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

bool QFileViewController::saveFile()
{
	bool bRes = true;

	if(!m_bIsFileOpen) {
		qWarning("[File] No file open");
		return bRes;
	} 
	QFile file(m_file.fileName());
	bRes = file.open(QIODevice::WriteOnly);
	if (bRes) {
		qint64 iBytesWritten = file.write(m_szData.toUtf8());

		if (iBytesWritten == -1) {
			qWarning("[File] Writing error");
		}
		file.close();
		qWarning("[File] Saved");
		return bRes;
	} else {
		qWarning("[File] Unable to save file: %s" , qPrintable(m_file.fileName()));
	}
	return bRes;
}

bool QFileViewController::readFile(qint64 iStartOffset)
{
	QSignalBlocker blocker(m_pFileView);

	bool bRes;
	m_iFilePos = iStartOffset;
	bRes = m_file.seek(m_iFilePos);
	char* pBuffer = new char[m_iBytePerLine];
	QString szTmp;
	int iNbRead;

	if(bRes){
		do {
			iNbRead = m_file.read(pBuffer, m_iBytePerLine);
			szTmp = QString("%0%1").arg(szTmp).arg(pBuffer);
		} while (iNbRead > 0);
		updateText(szTmp, iStartOffset);
	}

	if(pBuffer){
		delete[] pBuffer;
		pBuffer = NULL;
	}

	return bRes;
}

void QFileViewController::updateText(QString szText, qint64 iStartOffset)
{
	m_iFilePos = iStartOffset;
	m_szData = szText;

	quint32 iOffset;
	qint64 iNbRead;
	QString szOffsetText;
	QString szHexText;
	QString szHumanText;
	QChar c;

	QString szTmp;
	if (szText.isEmpty()) {
		return;
	}
	for(int i=0; i<m_iVisibleRowCount; i++){
		// Prepend a line break if not first row
		if(i>0){
			szOffsetText += "\n";
			if (szHexText.at(szHexText.length() - 1) != "\n") {
				szHexText += "\n";
			}
			if (szHumanText.at(szHumanText.length() - 1) != "\n") {
				szHumanText += "\n";
			}
		}

		iOffset = (quint32)(m_iFilePos+i*m_iBytePerLine);
		QStringASPrintf(szTmp, "0x%08X", iOffset);
		szOffsetText += szTmp;
		iNbRead = std::min(m_szData.length() - i * m_iBytePerLine - m_iFilePos, (qint64)m_iBytePerLine);
		for(int j=0; j<iNbRead; j++)
		{
			// Set hex text
			c = m_szData.at(i * m_iBytePerLine + j + m_iFilePos);
			QStringASPrintf(szTmp, "%02X", (unsigned char)(static_cast<char>(c.unicode())));
			szHexText += szTmp;
			if (j < iNbRead - 1) {
				szHexText += " ";
			}

			// Set human text
			if(c >= 0x20 && c <= 0x7E){
				szHumanText += c;
			}else{
				szHumanText += ".";
			}
		}
	}
	m_pFileView->setOffsetText(szOffsetText);
	m_pFileView->setHexText(szHexText);
	m_pFileView->setHumanText(szHumanText);

	m_iFileSize = m_szData.length();
	m_iTotalRowCount = ceil(m_iFileSize / float(m_iBytePerLine));
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
	if(iFirstVisibleRow < iRowStart){
		iPosStart += (iRowStart-iFirstVisibleRow) * m_iBytePerLine;
	}

	int iNbSelectedLine = (iRowEnd-iRowStart);
	iNbSelectedLine++;

	m_pFileView->moveToRow(iFirstVisibleRow);
	m_pFileView->selectText(iPosStart, iPosStart + (int)(iSize), (iRowStart-iFirstVisibleRow), iNbSelectedLine);
}

QString QFileViewController::getStringData()
{
	return m_szData;
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
	updateText(m_szData, m_iFilePos);
}

void QFileViewController::moveToRow(int iRow)
{
	QSignalBlocker block(m_pFileView);

	iRow = qMin(iRow, m_iTotalRowCount - m_iVisibleRowCount);

	qint64 iOffset = (((qint64)iRow) * m_iBytePerLine);
	updateText(m_szData, iOffset);
}

void QFileViewController::handleTextChangedHex(QPlainTextEdit* pHexEditor, QPlainTextEdit* pHumanEditor) 
{
	QSignalBlocker block(m_pFileView);
	
	QTextCursor tHexCursor = pHexEditor->textCursor();
	int iSelectionStart = tHexCursor.selectionStart();
	QString szHexText = pHexEditor->toPlainText();

	QString szTmp;
	if (szHexText.mid(tHexCursor.selectionStart() - 1, 2).contains(" ") || szHexText.mid(tHexCursor.selectionStart() - 1, 2).contains("\n")) { // AA |A|A
		szTmp = szHexText.mid(tHexCursor.selectionStart() - 2, 2);

	} else { // A|A| AA
		szTmp = szHexText.mid(tHexCursor.selectionStart() - 1, 2);
	}

	bool bOk;
	int iText = szTmp.toInt(&bOk, 16);
	if (bOk) {
		char cRes = static_cast<char>(iText);
		m_szData.remove(tHexCursor.position() / 3 + m_iFilePos, 1);
		m_szData.insert(tHexCursor.position() / 3 + m_iFilePos, cRes);
	}
	updateText(m_szData, m_iFilePos);

	tHexCursor.setPosition(iSelectionStart);
	tHexCursor.setPosition(iSelectionStart + 1, QTextCursor::KeepAnchor);
	pHexEditor->setTextCursor(tHexCursor);
}	

void QFileViewController::handleTextChangedHuman(QPlainTextEdit* pHumanEditor, QPlainTextEdit* pHexEditor) 
{
	QSignalBlocker blocker(m_pFileView);

	QTextCursor tHumanCursor = pHumanEditor->textCursor();
	int iSelectionStart = tHumanCursor.selectionStart();
	int iNbEnter = pHumanEditor->toPlainText().mid(0, tHumanCursor.selectionStart()).count("\n");
	
	char cRes = pHumanEditor->toPlainText().mid(tHumanCursor.position() - 1, 1).at(0).toLatin1();
	m_szData.remove(tHumanCursor.selectionStart() - 1 - iNbEnter + m_iFilePos, 1);
	m_szData.insert(tHumanCursor.selectionStart() - 1 - iNbEnter + m_iFilePos, cRes);

	updateText(m_szData, m_iFilePos);

	tHumanCursor.setPosition(iSelectionStart);
	tHumanCursor.setPosition(iSelectionStart + 1, QTextCursor::KeepAnchor);
	pHumanEditor->setTextCursor(tHumanCursor);
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

void QFileViewController::addNewByteHex(QPlainTextEdit* pHexEditor)
{
	QSignalBlocker block(m_pFileView);
	
	QTextCursor tHexCursor = pHexEditor->textCursor();
	int iSelectionStart = tHexCursor.selectionStart();
	QString szHexText = pHexEditor->toPlainText();

	QString szTmp = "00";

	bool bOk;
	int iText = szTmp.toInt(&bOk, 16);
	if (bOk) {
		char cRes = static_cast<char>(iText);
		m_szData.insert(tHexCursor.position() / 3 + m_iFilePos, cRes);
	}
	updateText(m_szData, m_iFilePos);

	tHexCursor.setPosition(iSelectionStart);
	tHexCursor.setPosition(iSelectionStart + 1, QTextCursor::KeepAnchor);
	pHexEditor->setTextCursor(tHexCursor);
}

void QFileViewController::removeByteHex(QPlainTextEdit* pHexEditor)
{
	QSignalBlocker block(m_pFileView);
	
	QTextCursor tHexCursor = pHexEditor->textCursor();
	int iSelectionStart = tHexCursor.selectionStart();
	QString szHexText = pHexEditor->toPlainText();

	m_szData.remove(tHexCursor.position() / 3 + m_iFilePos, 1);

	updateText(m_szData, m_iFilePos);

	tHexCursor.setPosition(iSelectionStart);
	tHexCursor.setPosition(iSelectionStart + 1, QTextCursor::KeepAnchor);
	pHexEditor->setTextCursor(tHexCursor);
}

void QFileViewController::addNewByteHuman(QPlainTextEdit* pHumanEditor, QString szText)
{
	QSignalBlocker blocker(m_pFileView);

	QTextCursor tHumanCursor = pHumanEditor->textCursor();
	int iSelectionStart = tHumanCursor.selectionStart();
	int iNbEnter = pHumanEditor->toPlainText().mid(0, tHumanCursor.selectionStart()).count("\n");
	
	m_szData.insert(tHumanCursor.selectionStart() - iNbEnter + m_iFilePos, szText);

	updateText(m_szData, m_iFilePos);

	tHumanCursor.setPosition(iSelectionStart + 1);
	pHumanEditor->setTextCursor(tHumanCursor);
}

void QFileViewController::removeByteHuman(QPlainTextEdit* pHumanEditor)
{
	QSignalBlocker blocker(m_pFileView);

	QTextCursor tHumanCursor = pHumanEditor->textCursor();
	int iSelectionStart = tHumanCursor.selectionStart();
	int iNbEnter = pHumanEditor->toPlainText().mid(0, tHumanCursor.selectionStart()).count("\n");
	
	m_szData.remove(tHumanCursor.selectionStart() - 1 - iNbEnter + m_iFilePos, 1);
	
	updateText(m_szData, m_iFilePos);

	tHumanCursor.setPosition(iSelectionStart + 1);
	pHumanEditor->setTextCursor(tHumanCursor);
}

void QFileViewController::findAllOccurrencesRegex(const QString &szSubString, QList<qint64>* plstPositions)
{
	QRegularExpression re(szSubString);
	QRegularExpressionMatchIterator i = re.globalMatch(m_szData);

	while (i.hasNext()) {
		QRegularExpressionMatch match = i.next();
		plstPositions->append(match.capturedStart());
	}
}

// - menu find
// - fenetre de gestion des structures, pour importer des fichiers de structure dans le dossier de l'utilisateur
// - ajouter un gestionnaire des fichiers de structure
// - ajouter la coloration (cf proposition de mathieu), activable et desactivable avec une option
