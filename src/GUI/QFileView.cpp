/*
 * QFileView.cpp
 *
 *  Created on: 18 juil. 2018
 *      Author: ebeuque
 */

#include <limits>
#include <math.h>

#include "QFileView.h"

#include <QHBoxLayout>
#include <QPlainTextEdit>
#include <QScrollBar>
#include <QKeyEvent>
#include <QObject>


QFileView::QFileView(QWidget* pParent)
	: QWidget(pParent)
{
    QBoxLayout* pMainLayout = new QHBoxLayout();
    setLayout(pMainLayout);

    QFont font;

    m_pOffsetEditor = new QPlainTextEdit(pParent);
    font = m_pOffsetEditor->font();
    font.setFamily("DejaVu Sans Mono");
    m_pOffsetEditor->setFont(font);
    m_pOffsetEditor->setFixedWidth(100);
    m_pOffsetEditor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pOffsetEditor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pOffsetEditor->setLineWrapMode(QPlainTextEdit::NoWrap);
	m_pOffsetEditor->setReadOnly(true);
    pMainLayout->addWidget(m_pOffsetEditor);

    m_pHexEditor = new QPlainTextEdit(pParent);
    font = m_pHexEditor->font();
    font.setFamily("DejaVu Sans Mono");
    m_pHexEditor->setFont(font);
    m_pHexEditor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pHexEditor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pHexEditor->setLineWrapMode(QPlainTextEdit::NoWrap);
	m_pHexEditor->installEventFilter(this);
    pMainLayout->addWidget(m_pHexEditor, 3);

	connect(m_pHexEditor, &QPlainTextEdit::textChanged, [this]() {
		emit textChangedHex(m_pHexEditor);
	});
	
	connect(m_pHexEditor, &QPlainTextEdit::selectionChanged, [this]() {
		emit selectionChangedHex(m_pHexEditor, m_pHumanEditor);
	});
	
	connect(m_pHexEditor, &QPlainTextEdit::cursorPositionChanged, [this]() {
		emit cursorChangedHex(m_pHexEditor, m_pHumanEditor);
	});

    m_pHumanEditor = new QPlainTextEdit(pParent);
    font = m_pHumanEditor->font();
    font.setFamily("DejaVu Sans Mono");
    m_pHumanEditor->setFont(font);
    m_pHumanEditor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pHumanEditor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pHumanEditor->setLineWrapMode(QPlainTextEdit::NoWrap);
	m_pHumanEditor->installEventFilter(this);
    pMainLayout->addWidget(m_pHumanEditor, 1);

	connect(m_pHumanEditor, &QPlainTextEdit::textChanged, [this]() {
		emit textChangedHuman(m_pHumanEditor);
	});
	
	connect(m_pHumanEditor, &QPlainTextEdit::selectionChanged, [this]() {
		emit selectionChangedHuman(m_pHumanEditor, m_pHexEditor);
	});
	
	connect(m_pHumanEditor, &QPlainTextEdit::cursorPositionChanged, [this]() {
		emit cursorChangedHuman(m_pHumanEditor, m_pHexEditor);
	});

    m_pScrollBar = new QScrollBar(pParent);
    pMainLayout->addWidget(m_pScrollBar);

    connect(m_pScrollBar, SIGNAL(valueChanged(int)), this, SIGNAL(rowChanged(int)));
}

QFileView::~QFileView()
{

}

void QFileView::setRowCount(int iMax)
{
	m_pScrollBar->setMaximum(iMax);
}

void QFileView::setCurrentRow(int iRow)
{
	m_pScrollBar->setValue(iRow);
}

int QFileView::getBytePerLine() const
{
	int iWidth = m_pHumanEditor->width();
	// Remove left and right margin
	iWidth -= m_pHumanEditor->document()->documentMargin();
	iWidth -= m_pHumanEditor->document()->documentMargin();
	iWidth -= 4;

	QFontMetrics fontMetrics = m_pHumanEditor->fontMetrics();
	int iPossibleBytePerLine = (int)floor(iWidth/(float)(fontMetrics.maxWidth()));
	return iPossibleBytePerLine;
}

int QFileView::getVisibleRowCount() const
{
	int iHeight;
	iHeight = m_pHumanEditor->height();
	// Remove top and bottom margin
	iHeight -= m_pHumanEditor->document()->documentMargin();
	iHeight -= m_pHumanEditor->document()->documentMargin();
	iHeight -= 4;

	QFontMetrics fontMetrics = m_pHumanEditor->fontMetrics();
	int iVisibleRowCount = (int)floor(iHeight/(float)(fontMetrics.height()));
	return iVisibleRowCount;
}

void QFileView::moveToRow(int iRow)
{
	m_pScrollBar->setValue(iRow);
}

void QFileView::selectText(int iPosStart, int iPosEnd, int iNbRowBefore, int iNbRowSelected)
{
	QTextCursor c;

	// iNbRowBefore and iNbRowSelected represents \n to be added
	int iNbSelectedLine = std::max(iNbRowBefore+iNbRowSelected-1, 0);

	c = m_pHumanEditor->textCursor();
	c.setPosition(iPosStart+iNbRowBefore);
	c.setPosition(iPosEnd+iNbSelectedLine, QTextCursor::KeepAnchor);
	m_pHumanEditor->setTextCursor(c);

	// Because have signal when selection changed
	// c = m_pHexEditor->textCursor();
	// c.setPosition(iPosStart*3+iNbRowBefore);
	// c.setPosition(iPosEnd*3+iNbSelectedLine-1, QTextCursor::KeepAnchor);  // -1 removes last space
	// m_pHexEditor->setTextCursor(c);
}

void QFileView::setOffsetText(const QString& szText)
{
	m_pOffsetEditor->setPlainText(szText);
}

void QFileView::setHexText(const QString& szText)
{
	m_pHexEditor->setPlainText(szText);
}

void QFileView::setHumanText(const QString& szText)
{
	m_pHumanEditor->setPlainText(szText);
}

void QFileView::resizeEvent(QResizeEvent *event)
{
	emit sizeChanged();
}

bool QFileView::eventHexEditor(QObject *obj, QEvent *event) 
{
	if (event->type() == QEvent::KeyPress) {
		QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
		QString allowedChars = "1234567890ABCDEF";
		QString keyText = keyEvent->text().toUpper();
		QTextCursor cursor = m_pHexEditor->textCursor();
		QString szEnter = "\n";
		
		bool isMovingKey = (keyEvent->key() == Qt::Key_Left ||
							keyEvent->key() == Qt::Key_Right ||
							keyEvent->key() == Qt::Key_Up ||
							keyEvent->key() == Qt::Key_Down);
		
		bool isDigitKey = keyEvent->key() >= Qt::Key_0 && keyEvent->key() <= Qt::Key_9;
		bool isTextKey = allowedChars.contains(keyText);
		int iSelectionStart;
		int iSelectionEnd;
		iSelectionStart = cursor.selectionStart();
		iSelectionEnd = cursor.selectionEnd();
		QString szHexText = m_pHexEditor->toPlainText();

		if (isMovingKey) {
			if (keyEvent->modifiers() == Qt::NoModifier) {
				switch(keyEvent->key()) {
					case Qt::Key_Left:
						cursor.movePosition(QTextCursor::Left);
						cursor.movePosition(QTextCursor::Left);
						break;
					case Qt::Key_Right:
						cursor.movePosition(QTextCursor::Right);
						break;
					case Qt::Key_Up:
						cursor.movePosition(QTextCursor::Up);
						break;
					case Qt::Key_Down:
						cursor.movePosition(QTextCursor::Down);
						break;
					default:
						break;
				}
				iSelectionStart = cursor.position();
				while (iSelectionStart > 0 && iSelectionStart < szHexText.length() && (szHexText.mid(iSelectionStart, 1) == " " || szHexText.mid(iSelectionStart, 1) == szEnter)) {
					iSelectionStart += (keyEvent->key() == Qt::Key_Right ? 1 : -1);
				}
				cursor.setPosition(iSelectionStart);
				cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
				m_pHexEditor->setTextCursor(cursor);
			} else if (keyEvent->modifiers() == Qt::ShiftModifier) {
				QTextCursor tHexCursor = m_pHexEditor->textCursor();
				if (tHexCursor.position() == tHexCursor.selectionEnd()) {
					iSelectionStart = tHexCursor.selectionStart();
					iSelectionEnd = tHexCursor.selectionEnd();
					if (keyEvent->key() == Qt::Key_Left) {
						iSelectionEnd -= 4;
					}
				} else {
					iSelectionStart = tHexCursor.selectionEnd();
					iSelectionEnd = tHexCursor.selectionStart();
					if (keyEvent->key() == Qt::Key_Right) {
						iSelectionEnd += 4;
					}
				}
				tHexCursor.setPosition(iSelectionStart);
				tHexCursor.setPosition(iSelectionEnd, QTextCursor::KeepAnchor);
				m_pHexEditor->setTextCursor(tHexCursor);
				return QWidget::eventFilter(obj, event);
			}
		} else if ((!keyText.isEmpty() && isTextKey && (keyEvent->modifiers() == Qt::NoModifier || keyEvent->modifiers() == Qt::ShiftModifier)) || isDigitKey) {
			if (cursor.hasSelection() && abs(cursor.selectionEnd() - cursor.selectionStart()) == 1) {
				cursor.insertText(keyText);
			} 
		}
		//  else if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return) {	
		// 	emit addNewByteHex(m_pHexEditor);
		// } else if (keyEvent->key() == Qt::Key_Backspace) {
		// 	emit removeByteHex(m_pHexEditor);
		// }
		return true;
	}
	return QWidget::eventFilter(obj, event);
}

bool QFileView::eventHumanEditor(QObject *obj, QEvent *event) 
{
	if (event->type() == QEvent::KeyPress) {
		QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
		QTextCursor cursor = m_pHumanEditor->textCursor();
		QString keyText = keyEvent->text();
		bool isMovingKey = (keyEvent->key() == Qt::Key_Left ||
							keyEvent->key() == Qt::Key_Right ||
							keyEvent->key() == Qt::Key_Up ||
							keyEvent->key() == Qt::Key_Down);

		if (isMovingKey) {
			return QWidget::eventFilter(obj, event);
		} 
		if (!keyText.isEmpty() && cursor.hasSelection() && abs(cursor.selectionEnd() - cursor.selectionStart()) == 1){
			return QWidget::eventFilter(obj, event);	
		}
		// if (!keyText.isEmpty() && !cursor.hasSelection() && !(keyEvent->key() == Qt::Key_Backspace)) {
		// 	emit addNewByteHuman(m_pHumanEditor, keyText);
		// } else if (keyEvent->key() == Qt::Key_Backspace) {
		// 	emit removeByteHuman(m_pHumanEditor);
		// }
		return true;
	}
	return QWidget::eventFilter(obj, event);
}

bool QFileView::eventFilter(QObject *obj, QEvent *event)
{
	if (obj == m_pHexEditor) {
		return eventHexEditor(obj, event);
	}
	
	if (obj == m_pHumanEditor) {
		return eventHumanEditor(obj, event);
	}
	
	return QWidget::eventFilter(obj, event);
}


QPlainTextEdit* QFileView::getHexEditor()
{
	return m_pHexEditor;
}
QPlainTextEdit* QFileView::getHumanEditor()
{
	return m_pHumanEditor;
}