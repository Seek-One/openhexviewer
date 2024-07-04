#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Global/QtCompat.h"
#include "QFindDialogController.h"
#include "GUI/QFileView.h"
#include "QFileViewController.h"

#include <QMessageBox>
#include <QPlainTextEdit>

#include <QFile>


QFindDialogController::QFindDialogController(QFindDialog* pFindDialog)
{
	m_pFindDialog = pFindDialog;
	m_iDataSize = 0;
	m_iDataPos = 0;

	m_bChanged = true;

	m_iBytePerLine = 0;
	m_iTotalRowCount = 0;
	m_iVisibleRowCount = 0;
	
	connect(m_pFindDialog, SIGNAL(sizeChanged()), this, SLOT(updateView()));
	connect(m_pFindDialog, SIGNAL(rowChanged(int)), this, SLOT(moveToRow(int)));

	connect(m_pFindDialog, SIGNAL(findNext()), this, SLOT(findNext()));
	connect(m_pFindDialog, SIGNAL(findPrevious()), this, SLOT(findPrevious()));

	connect(m_pFindDialog, &QFindDialog::offsetChanged, [this]() {
		m_bChanged = true;
	});
	connect(m_pFindDialog, QOverload<int>::of(&QFindDialog::comboOffsetChanged), [this](int iIndex) {
		QSignalBlocker block(m_pFindDialog);
		m_iListIndex = iIndex;
		m_pFindDialog->setLabelNbOcc(m_iListIndex + 1, m_iPositionsSize);
		emit selectData(m_lstPositions[m_iListIndex], m_byteArray.length());
	});
	
	connect(m_pFindDialog, SIGNAL(selectionChangedHex(QPlainTextEdit*, QPlainTextEdit*)), this, SLOT(handleSelectionChangedHex(QPlainTextEdit*, QPlainTextEdit*)));
	connect(m_pFindDialog, SIGNAL(selectionChangedHuman(QPlainTextEdit*, QPlainTextEdit*)), this, SLOT(handleSelectionChangedHuman(QPlainTextEdit*, QPlainTextEdit*)));
	connect(m_pFindDialog, SIGNAL(removeHumanEditor(QPlainTextEdit*)), this, SLOT(removeHumanEditor(QPlainTextEdit*)));
	connect(m_pFindDialog, SIGNAL(removeHexEditor(QPlainTextEdit*)), this, SLOT(removeHexEditor(QPlainTextEdit*)));

	connect(m_pFindDialog, SIGNAL(insertCharHumanEditor(QPlainTextEdit*, QString&)), this, SLOT(insertCharHumanEditor(QPlainTextEdit*, QString&)));
	connect(m_pFindDialog, SIGNAL(insertCharHexEditor(QPlainTextEdit*, QString&)), this, SLOT(insertCharHexEditor(QPlainTextEdit*, QString&)));
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
	if (m_lstPositions.isEmpty() || m_iPositionsSize <= 0 || m_bChanged) {
		return find();
	}
	(++m_iListIndex) %= m_iPositionsSize;
	m_pFindDialog->setLabelNbOcc(m_iListIndex + 1, m_iPositionsSize);
	
	emit selectData(m_lstPositions.at(m_iListIndex), m_byteArray.length());
}

void QFindDialogController::findPrevious()
{
	if (m_lstPositions.isEmpty() || m_iPositionsSize <= 0 || m_bChanged) {
		return find();
	}
	if (--m_iListIndex < 0) {
		m_iListIndex = m_iPositionsSize - 1;
	}
	m_pFindDialog->setLabelNbOcc(m_iListIndex + 1, m_iPositionsSize);

	emit selectData(m_lstPositions.at(m_iListIndex), m_byteArray.length());
}

void QFindDialogController::find()
{
	QSignalBlocker block(m_pFindDialog);

	m_lstPositions.clear();
	m_bChanged = false;
	OffsetValue oValues = m_pFindDialog->getOffsets();
	Value vStartOffset = oValues.startOffset;
	Value vEndOffset = oValues.endOffset;
	QString szTmp;
	bool bOk;
	qint64 iStartOffset = 0;
	qint64 iEndOffset = -1; //THE END
	if (vStartOffset.type == CUSTOM) {
		QString szText = m_pFindDialog->getStartOffset();
		iStartOffset = szText.toInt(&bOk, vStartOffset.base);
	}
	if (vEndOffset.type == CUSTOM) {
		QString szText = m_pFindDialog->getEndOffset();
		iEndOffset = szText.toInt(&bOk, vEndOffset.base);
	}

	if (iEndOffset != -1 && iStartOffset > iEndOffset) {
		qint64 iTmp = iStartOffset;
		iStartOffset = iEndOffset;
		iEndOffset = iTmp;
	}

	emit findAllOccurrencesRegex(m_byteArray, &m_lstPositions, iStartOffset, iEndOffset);
	m_iListIndex = 0;
	m_iPositionsSize = m_lstPositions.size(); 
	if (m_iPositionsSize > 0) {
		QComboBox* pComboBox = m_pFindDialog->getComboPosOcc();
		pComboBox->clear();
		for (int i = 0; i < m_iPositionsSize; i++) {
			szTmp = QString("0x%1 (%2)").arg(m_lstPositions[i], 0, 16).arg(m_lstPositions[i]);
			pComboBox->addItem(szTmp);
		}
		m_pFindDialog->setLabelNbOcc(m_iListIndex + 1, m_iPositionsSize);
	} else {
		m_pFindDialog->setLabelNbOcc(-1, -1);
	}
}

void QFindDialogController::updateText(qint64 iStartOffset)
{
	QSignalBlocker block(m_pFindDialog);

	m_iDataPos = iStartOffset;

	qint64 iNbRead;
	QString szHexText;
	QString szHumanText;
	QChar c;

	QString szTmp;
	if (m_byteArray.isEmpty()) {
		m_pFindDialog->setHexText("");
		m_pFindDialog->setHumanText("");
		return;
	}
	for(int i=0; i<m_iVisibleRowCount; i++){
		// Prepend a line break if not first row
		if(i>0){
			if (m_byteArray.length() - m_iDataPos >= i * m_iBytePerLine) {
				szHexText += "\n";
				szHumanText += "\n";
			}
		}

		iNbRead = std::min(m_byteArray.length() - i * m_iBytePerLine - m_iDataPos, (qint64)m_iBytePerLine);
		for(int j=0; j<iNbRead; j++)
		{
			// Set hex text
			c = m_byteArray.at(i * m_iBytePerLine + j + m_iDataPos);
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

	m_iDataSize = m_byteArray.length();
	m_iTotalRowCount = ceil(m_iDataSize / float(m_iBytePerLine));
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

void QFindDialogController::insertCharHexEditor(QPlainTextEdit* pHexEditor, QString& keyText)
{
	m_bChanged = true;
	
	QTextCursor c = pHexEditor->textCursor();
	int iPosition = c.selectionStart();
	QString szText = pHexEditor->toPlainText();
	QString szTmp;
	bool bOk;
	bool hasCorrectSelection = c.hasSelection() && c.selectionEnd() - c.selectionStart() == 1; 
	bool isAtTheEnd = c.selectionStart() == szText.length();

	if (hasCorrectSelection) {
		if (szText.mid(c.selectionStart() - 1, 1).contains(" ") || szText.mid(c.selectionStart() - 1, 1).contains("\n")) { // |C|A
			szTmp = keyText + szText.mid(c.selectionStart() + 1, 1);
		} else { // A|C|
			szTmp = szText.mid(c.selectionStart() - 1, 1) + keyText;
		}
		int iText = szTmp.toInt(&bOk, 16);
		if (bOk) {
			m_byteArray[(int)(c.selectionStart() / 3 + m_iDataPos)] = static_cast<char>(iText);
		} else {
			QMessageBox::critical(this, "", tr("Conversion problem"), QMessageBox::Ok, QMessageBox::Ok);
			return;
		}
	} else {
		if (c.hasSelection()) {
			m_byteArray.remove(c.selectionStart() / 3 + m_iDataPos, (c.selectionEnd() - c.selectionStart()) / 3 + 1);
		}
		szTmp = keyText + "0";
		int iText = szTmp.toInt(&bOk, 16);
		if (bOk) {
			m_byteArray.insert((c.selectionStart() + 1) / 3 + m_iDataPos, static_cast<char>(iText));
		}	else {
			QMessageBox::critical(this, "", tr("Conversion problem"), QMessageBox::Ok, QMessageBox::Ok);
			return;
		}
	}

	updateText(m_iDataPos);

	if (isAtTheEnd && iPosition % (m_iBytePerLine * 3) != 0) {
		iPosition ++;
	}

	szText = pHexEditor->toPlainText();
	while (szText.mid(iPosition + 1, 1).contains(" ") || szText.mid(iPosition + 1, 1).contains("\n")) {
		iPosition++;
	}
	c.setPosition(iPosition + 1);
	c.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);

	
	pHexEditor->setTextCursor(c);
}

void QFindDialogController::removeHexEditor(QPlainTextEdit* pHexEditor)
{
	QSignalBlocker block(m_pFindDialog);
	
	m_bChanged = true;
	
	QTextCursor c = pHexEditor->textCursor();
	int iPosition = c.selectionStart();
	QString szText = pHexEditor->toPlainText();
	bool hasCorrectSelection = c.hasSelection() && c.selectionEnd() - c.selectionStart() > 1;
	
	if (hasCorrectSelection) {
		m_byteArray.remove(c.selectionStart() / 3, (c.selectionEnd() - c.selectionStart()) / 3 + 1);	
	} else {
		m_byteArray.remove(c.selectionStart() / 3 + m_iDataPos, 1);
	}
	updateText(m_iDataPos);

	if (hasCorrectSelection) {
		c.setPosition(iPosition);
	} else {
		--iPosition;
		if (iPosition >= pHexEditor->toPlainText().length()) {
			iPosition =	pHexEditor->toPlainText().length() - 1;
		}
		if(iPosition < 0) {
			iPosition = 0;
		}
		c.setPosition(iPosition);
	}
	pHexEditor->setTextCursor(c);
}

void QFindDialogController::insertCharHumanEditor(QPlainTextEdit* pHumanEditor, QString& keyText)
{	
	m_bChanged = true;
	
	QTextCursor c = pHumanEditor->textCursor();
	int iPosition = c.selectionStart();
	QString szText = pHumanEditor->toPlainText();
	int iNbEnter = szText.mid(0, c.selectionStart()).count("\n");

	if (szText.mid(c.selectionStart(), 1).contains("\n")) {
		iPosition++;
	}

	if (c.hasSelection()) {
		m_byteArray.remove(c.selectionStart() + m_iDataPos, c.selectionEnd() - c.selectionStart());
	}
	m_byteArray.insert(c.selectionStart() + m_iDataPos - iNbEnter, keyText.at(0).toLatin1());
	
	updateText(m_iDataPos);

	c.setPosition(iPosition + 1);
	pHumanEditor->setTextCursor(c);
}

void QFindDialogController::removeHumanEditor(QPlainTextEdit* pHumanEditor)
{
	QSignalBlocker block(m_pFindDialog);
	
	m_bChanged = true;
	
	QTextCursor c = pHumanEditor->textCursor();
	int iPosition = c.selectionStart();
	QString szText = pHumanEditor->toPlainText();
	int iNbEnter = szText.mid(0, c.selectionStart()).count("\n");
	int iNbEnterSelection = szText.mid(c.selectionStart(), c.selectionEnd() - c.selectionStart()).count("\n");
	bool hasSelection = c.hasSelection();
	if (hasSelection) {
		m_byteArray.remove(c.selectionStart() + m_iDataPos - iNbEnter, c.selectionEnd() - c.selectionStart() - iNbEnterSelection);
	} else {
		m_byteArray.remove(c.selectionStart() + m_iDataPos - 1 - iNbEnter, 1);
	}
	updateText(m_iDataPos);
	if (hasSelection) {
		c.setPosition(iPosition);
	} else {
		iPosition -= 1;
		if (iPosition < 0) {
			iPosition = 1;
		}
		c.setPosition(iPosition);
	}
	pHumanEditor->setTextCursor(c);
}

QByteArray QFindDialogController::getStringData()
{
    return m_byteArray;
}