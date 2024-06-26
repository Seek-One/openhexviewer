#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Global/QtCompat.h"
#include "QFindDialogController.h"
#include "../GUI/QFileView.h"
#include "QFileViewController.h" // Include the full definition


#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QIcon>
#include <QToolButton>
#include <QPushButton>
#include <QPlainTextEdit>

#include <QFile>


QFindDialogController::QFindDialogController(QFindDialog* pFindDialog)
{
	m_pFindDialog = pFindDialog;
	m_iDataSize = 0;
	m_iDataPos = 0;

	m_iBytePerLine = 0;
	m_iTotalRowCount = 0;
	m_iVisibleRowCount = 0;
	
	connect(m_pFindDialog, SIGNAL(sizeChanged()), this, SLOT(updateView()));
	connect(m_pFindDialog, SIGNAL(rowChanged(int)), this, SLOT(moveToRow(int)));

	connect(m_pFindDialog, SIGNAL(accepted()), this, SLOT(find()));
	connect(m_pFindDialog, SIGNAL(findNext()), this, SLOT(findNext()));
	connect(m_pFindDialog, SIGNAL(findPrevious()), this, SLOT(findPrevious()));
	
	connect(m_pFindDialog, SIGNAL(textChangedHex(QPlainTextEdit*)), this, SLOT(handleTextChangedHex(QPlainTextEdit*)));
	connect(m_pFindDialog, SIGNAL(textChangedHuman(QPlainTextEdit*)), this, SLOT(handleTextChangedHuman(QPlainTextEdit*)));
	connect(m_pFindDialog, SIGNAL(selectionChangedHex(QPlainTextEdit*, QPlainTextEdit*)), this, SLOT(handleSelectionChangedHex(QPlainTextEdit*, QPlainTextEdit*)));
	connect(m_pFindDialog, SIGNAL(selectionChangedHuman(QPlainTextEdit*, QPlainTextEdit*)), this, SLOT(handleSelectionChangedHuman(QPlainTextEdit*, QPlainTextEdit*)));
	connect(m_pFindDialog, SIGNAL(cursorChangedHex(QPlainTextEdit*, QPlainTextEdit*)), this, SLOT(handleCursorChangedHex(QPlainTextEdit*, QPlainTextEdit*)));
	connect(m_pFindDialog, SIGNAL(cursorChangedHuman(QPlainTextEdit*, QPlainTextEdit*)), this, SLOT(handleCursorChangedHuman(QPlainTextEdit*, QPlainTextEdit*)));
	connect(m_pFindDialog, SIGNAL(removeHumanEditor(QPlainTextEdit*)), this, SLOT(removeHumanEditor(QPlainTextEdit*)));
	connect(m_pFindDialog, SIGNAL(removeHexEditor(QPlainTextEdit*)), this, SLOT(removeHexEditor(QPlainTextEdit*)));
}

QFindDialogController::~QFindDialogController()
{

}

void QFindDialogController::updateDisplayData()
{
	m_iBytePerLine = m_pFindDialog->getBytePerLine();
	m_iTotalRowCount = ceil(m_iDataSize / (float)m_iBytePerLine);
	m_iVisibleRowCount = m_pFindDialog->getVisibleRowCount();
	m_pFindDialog->setRowCount(m_iTotalRowCount);
}

void QFindDialogController::updateView()
{	
	QSignalBlocker block(m_pFindDialog);

	updateDisplayData();
	updateText(m_iDataPos);
}

void QFindDialogController::moveToRow(int iRow)
{
	QSignalBlocker block(m_pFindDialog);

	iRow = qMin(iRow, m_iTotalRowCount - m_iVisibleRowCount);

	qint64 iOffset = (((qint64)iRow) * m_iBytePerLine);
	updateText(iOffset);
}

void QFindDialogController::findNext()
{
	if (m_lstPositions.isEmpty() || m_iPositionsSize <= 0) {
		return find();
	}
	(++m_iListIndex) %= m_iPositionsSize;
	m_pFindDialog->setLabelNbOcc(m_iListIndex + 1, m_iPositionsSize);
	
	emit selectData(m_lstPositions.at(m_iListIndex), m_szData.length());
}

void QFindDialogController::findPrevious()
{
	if (m_lstPositions.isEmpty() || m_iPositionsSize <= 0) {
		return find();
	}
	if (--m_iListIndex < 0) {
		m_iListIndex = m_iPositionsSize - 1;
	}
	m_pFindDialog->setLabelNbOcc(m_iListIndex + 1, m_iPositionsSize);

	emit selectData(m_lstPositions.at(m_iListIndex), m_szData.length());
}

void QFindDialogController::find()
{
	m_lstPositions.clear();
	m_iFindSize = m_szData.length();

	emit findAllOccurrencesRegex(m_szData, &m_lstPositions);
	m_iListIndex = 0;
	m_iPositionsSize = m_lstPositions.size(); 
	if (m_iPositionsSize > 0) {
		m_pFindDialog->setLabelNbOcc(m_iListIndex + 1, m_iPositionsSize);
	} else {
		m_pFindDialog->setLabelNbOcc(-1, -1);
	}
}

void QFindDialogController::updateText(qint64 iStartOffset)
{
	m_iDataPos = iStartOffset;

	qint64 iNbRead;
	QString szHexText;
	QString szHumanText;
	QChar c;

	QString szTmp;
	if (m_szData.isEmpty()) {
		m_pFindDialog->setHexText("");
		m_pFindDialog->setHumanText("");
		return;
	}
	for(int i=0; i<m_iVisibleRowCount; i++){
		// Prepend a line break if not first row
		if(i>0){
			if (m_szData.length() - m_iDataPos >= i * m_iBytePerLine) {
				szHexText += "\n";
				szHumanText += "\n";
			}
		}

		iNbRead = std::min(m_szData.length() - i * m_iBytePerLine - m_iDataPos, (qint64)m_iBytePerLine);
		for(int j=0; j<iNbRead; j++)
		{
			// Set hex text
			c = m_szData.at(i * m_iBytePerLine + j + m_iDataPos);
			QStringASPrintf(szTmp, "%02X", (unsigned char)(static_cast<char>(c.unicode())));
			szHexText += szTmp;
			if (j < iNbRead - 1) {
				szHexText += " ";
			}

			// Set human text
			if(c.unicode() >= 0x20 && c.unicode() <= 0x7E){
				szHumanText += c;
			}else{
				szHumanText += ".";
			}
		}
	}
	m_pFindDialog->setHexText(szHexText);
	m_pFindDialog->setHumanText(szHumanText);

	m_iDataSize = m_szData.length();
	m_iTotalRowCount = ceil(m_iDataSize / float(m_iBytePerLine));
}

void QFindDialogController::handleTextChangedHex(QPlainTextEdit* pHexEditor) 
{
	QSignalBlocker block(m_pFindDialog);

	QTextCursor tHexCursor = pHexEditor->textCursor();
	int iSelectionStart = tHexCursor.selectionStart();
	int iStartHex = tHexCursor.selectionStart() - 1;
	QString szHexText = pHexEditor->toPlainText();
	
	while (iStartHex > 0 && !(szHexText.mid(iStartHex - 1, 1).contains(" ")) && !(szHexText.mid(iStartHex - 1, 1).contains("\n"))) {
		--iStartHex;
	}
	QString szTmp = szHexText.mid(iStartHex, 2);

	bool bOk;
	int iText = szTmp.toInt(&bOk, 16);
	if (bOk) {
		char cRes = static_cast<char>(iText);
		m_szData.remove(tHexCursor.position() / 3 + m_iDataPos, 1);
		m_szData.insert(tHexCursor.position() / 3 + m_iDataPos, cRes);
	}
	while (iSelectionStart < szHexText.length() && (szHexText.mid(iSelectionStart, 1).contains(" ") || szHexText.mid(iSelectionStart, 1).contains("\n"))) {
		++iSelectionStart;
	}

	if (iSelectionStart == szHexText.length()) {
		m_szData.append("0");
	}

	updateText(m_iDataPos);

	tHexCursor.setPosition(iSelectionStart);
	tHexCursor.setPosition(iSelectionStart + 1, QTextCursor::KeepAnchor);
	pHexEditor->setTextCursor(tHexCursor);
}	

void QFindDialogController::handleTextChangedHuman(QPlainTextEdit* pHumanEditor) 
{
	QSignalBlocker blocker(m_pFindDialog);

	QTextCursor tHumanCursor = pHumanEditor->textCursor();
	int iSelectionStart = tHumanCursor.selectionStart();
	QString szHumanText = pHumanEditor->toPlainText();
	int iNbEnter = szHumanText.mid(0, tHumanCursor.selectionStart()).count("\n");
	
	char cRes = szHumanText.mid(tHumanCursor.position() - 1, 1).at(0).toLatin1();
	m_szData.remove(tHumanCursor.selectionStart() - 1 - iNbEnter + m_iDataPos, 1);
	m_szData.insert(tHumanCursor.selectionStart() - 1 - iNbEnter + m_iDataPos, cRes);

	updateText(m_iDataPos);

	szHumanText = pHumanEditor->toPlainText();
	while (iSelectionStart < szHumanText.length() && szHumanText.mid(iSelectionStart, 1).contains("\n")) {
		++iSelectionStart;
	}
	tHumanCursor.setPosition(iSelectionStart);
	tHumanCursor.setPosition(iSelectionStart + 1, QTextCursor::KeepAnchor);
	pHumanEditor->setTextCursor(tHumanCursor);
}

void QFindDialogController::handleSelectionChangedHex(QPlainTextEdit* pHexEditor, QPlainTextEdit* pHumanEditor) 
{
	QSignalBlocker blocker(m_pFindDialog);

	QTextCursor tHexCursor = pHexEditor->textCursor();

	int iSelectionStart = tHexCursor.selectionStart();
	int iSelectionEnd = tHexCursor.selectionEnd();

	int iNbEnterStart = pHexEditor->toPlainText().mid(0, iSelectionStart).count("\n");
	int iNbEnterEnd = pHexEditor->toPlainText().mid(iSelectionStart, abs(iSelectionEnd - iSelectionStart)).count("\n");

	QTextCursor c = pHumanEditor->textCursor();
	c.setPosition(iSelectionStart / 3 + iNbEnterStart);
	c.setPosition(iSelectionEnd / 3 + iNbEnterEnd + iNbEnterStart + 1, QTextCursor::KeepAnchor);
	pHumanEditor->setTextCursor(c);
}

void QFindDialogController::handleSelectionChangedHuman(QPlainTextEdit* pHumanEditor, QPlainTextEdit* pHexEditor) 
{
	QSignalBlocker blocker(m_pFindDialog);

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
}

void QFindDialogController::handleCursorChangedHex(QPlainTextEdit* pHexEditor, QPlainTextEdit* pHumanEditor) 
{
	QSignalBlocker blocker(m_pFindDialog);
}

void QFindDialogController::handleCursorChangedHuman(QPlainTextEdit* pHumanEditor, QPlainTextEdit* pHexEditor) 
{
	QSignalBlocker blocker(m_pFindDialog);
}

void QFindDialogController::removeHexEditor(QPlainTextEdit* pHexEditor)
{
	QSignalBlocker block(m_pFindDialog);
	
	QTextCursor tHexCursor = pHexEditor->textCursor();
	int iSelectionStart = tHexCursor.selectionStart();
	int iSelectionEnd = tHexCursor.selectionEnd();
	int iNbEnter = pHexEditor->toPlainText().mid(0, tHexCursor.selectionStart()).count("\n");
	if (abs(iSelectionEnd - iSelectionStart) > 1) {
		m_szData.remove(tHexCursor.selectionStart() / 3 - iNbEnter + m_iDataPos, abs(iSelectionEnd - iSelectionStart) / 3 + 1);
	} else {
		m_szData.remove(tHexCursor.selectionStart() / 3 + m_iDataPos, 1);
	}
	updateText(m_iDataPos);

	tHexCursor.setPosition(iSelectionStart);
	pHexEditor->setTextCursor(tHexCursor);
}

void QFindDialogController::removeHumanEditor(QPlainTextEdit* pHumanEditor)
{
	QSignalBlocker block(m_pFindDialog);
	
	QTextCursor tHumanCursor = pHumanEditor->textCursor();
	int iSelectionStart = tHumanCursor.selectionStart();
	int iSelectionEnd = tHumanCursor.selectionEnd();
	int iNbEnter = pHumanEditor->toPlainText().mid(0, tHumanCursor.selectionStart()).count("\n");
	if (tHumanCursor.hasSelection()) {
		m_szData.remove(tHumanCursor.selectionStart() - iNbEnter + m_iDataPos, abs(iSelectionEnd - iSelectionStart));
		tHumanCursor.setPosition(iSelectionStart);
	} else {
		m_szData.remove(tHumanCursor.selectionStart() - 1 - iNbEnter + m_iDataPos, 1);
		tHumanCursor.setPosition(iSelectionStart - 1);
	}
	updateText(m_iDataPos);

	pHumanEditor->setTextCursor(tHumanCursor);
}

QString QFindDialogController::getStringData()
{
    return m_szData;
}