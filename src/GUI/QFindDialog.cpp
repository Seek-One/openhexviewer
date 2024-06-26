#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <limits>
#include <math.h>

#include "QFindDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollBar>
#include <QPlainTextEdit>


QFindDialog::QFindDialog(QWidget * pParent)
{
    QVBoxLayout* pMainLayout = new QVBoxLayout();
    setLayout(pMainLayout);

    {
        QBoxLayout* pViewLayout = new QHBoxLayout();
        pMainLayout->addLayout(pViewLayout);

        QFont font;

        m_pHexEditor = new QPlainTextEdit(pParent);
        font = m_pHexEditor->font();
        font.setFamily("DejaVu Sans Mono");
        m_pHexEditor->setFont(font);
        m_pHexEditor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_pHexEditor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_pHexEditor->setLineWrapMode(QPlainTextEdit::NoWrap);
        m_pHexEditor->installEventFilter(this);
        pViewLayout->addWidget(m_pHexEditor, 3);

        m_pHumanEditor = new QPlainTextEdit(pParent);
        font = m_pHumanEditor->font();
        font.setFamily("DejaVu Sans Mono");
        m_pHumanEditor->setFont(font);
        m_pHumanEditor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_pHumanEditor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_pHumanEditor->setLineWrapMode(QPlainTextEdit::NoWrap);
        m_pHumanEditor->installEventFilter(this);
        pViewLayout->addWidget(m_pHumanEditor, 1);

        m_pScrollBar = new QScrollBar(pParent);
        pViewLayout->addWidget(m_pScrollBar);

        connect(m_pScrollBar, SIGNAL(valueChanged(int)), this, SIGNAL(rowChanged(int)));

        connect(m_pHexEditor, &QPlainTextEdit::textChanged, [this]() {
            emit textChangedHex(m_pHexEditor);
        });
        // HEX TEXT CHANGED
        connect(m_pHexEditor, &QPlainTextEdit::textChanged, [this]() {
            emit textChangedHex(m_pHexEditor);
        });
        // HEX SELECTION CHANGED
        connect(m_pHexEditor, &QPlainTextEdit::selectionChanged, [this]() {
            emit selectionChangedHex(m_pHexEditor, m_pHumanEditor);
        });
        // HEX CURSOR CHANGED
        connect(m_pHexEditor, &QPlainTextEdit::cursorPositionChanged, [this]() {
            emit cursorChangedHex(m_pHexEditor, m_pHumanEditor);
        });

        //HUMAN TEXT CHANGED 
        connect(m_pHumanEditor, &QPlainTextEdit::textChanged, [this]() {
            emit textChangedHuman(m_pHumanEditor);
        });
        //HUMAN SELECTION CHANGED
        connect(m_pHumanEditor, &QPlainTextEdit::selectionChanged, [this]() {
            emit selectionChangedHuman(m_pHumanEditor, m_pHexEditor);
        });
        //HUMAN CURSOR CHANGED
        connect(m_pHumanEditor, &QPlainTextEdit::cursorPositionChanged, [this]() {
            emit cursorChangedHuman(m_pHumanEditor, m_pHexEditor);
        });
    }

    {
        QHBoxLayout* pButtonLayout = new QHBoxLayout();
        pMainLayout->addLayout(pButtonLayout);
        
        m_pButtonFindPrevious = new QPushButton(tr("Find Previous"));
        pButtonLayout->addWidget(m_pButtonFindPrevious);

        m_pLabelNbOcc = new QLabel("");
        pButtonLayout->addWidget(m_pLabelNbOcc);

        m_pButtonFindNext = new QPushButton(tr("Find Next"));
        pButtonLayout->addWidget(m_pButtonFindNext);
        m_pButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        pButtonLayout->addWidget(m_pButtonBox);

        connect(m_pButtonBox, SIGNAL(rejected()), this, SLOT(reject()));
        connect(m_pButtonBox, &QDialogButtonBox::accepted, [this]() {
            emit accepted();
        });

        connect(m_pButtonFindNext, &QAbstractButton::clicked, [this]() {
            emit findNext();
        });

        connect(m_pButtonFindPrevious, &QAbstractButton::clicked, [this]() {
            emit findPrevious();
        });
    }
}

QFindDialog::~QFindDialog()
{

}

void QFindDialog::setRowCount(int iMax)
{
	m_pScrollBar->setMaximum(iMax);
}

int QFindDialog::getBytePerLine() const
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

int QFindDialog::getVisibleRowCount() const
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

void QFindDialog::moveToRow(int iRow)
{
	m_pScrollBar->setValue(iRow);
}

void QFindDialog::setHexText(const QString& szText)
{
	m_pHexEditor->setPlainText(szText);
}

void QFindDialog::setHumanText(const QString& szText)
{
	m_pHumanEditor->setPlainText(szText);
}

void QFindDialog::resizeEvent(QResizeEvent *event)
{
	emit sizeChanged();
}

void QFindDialog::setLabelNbOcc(qint64 iIndexOcc, qint64 iNbOcc) 
{
    if (iIndexOcc < 0 && iNbOcc < 0) {
        m_pLabelNbOcc->setText("");
    } else {
        m_pLabelNbOcc->setText(QString("%0/%1").arg(iIndexOcc).arg(iNbOcc));
    }
}

bool QFindDialog::eventHexEditor(QObject *obj, QEvent *event) 
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
		} else if (!keyText.isEmpty() && isTextKey && (keyEvent->modifiers() == Qt::NoModifier || keyEvent->modifiers() == Qt::ShiftModifier) && keyEvent->key() != Qt::Key_Backspace) {
            cursor.insertText(keyText);
		} else if (keyEvent->key() == Qt::Key_Backspace) {
            emit removeHexEditor(m_pHexEditor);
        }
		return true;
	}
	return QWidget::eventFilter(obj, event);
}

bool QFindDialog::eventHumanEditor(QObject *obj, QEvent *event) 
{
	if (event->type() == QEvent::KeyPress) {
		QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
		QString keyText = keyEvent->text();
		bool isMovingKey = (keyEvent->key() == Qt::Key_Left ||
							keyEvent->key() == Qt::Key_Right ||
							keyEvent->key() == Qt::Key_Up ||
							keyEvent->key() == Qt::Key_Down);

		if ((isMovingKey || !keyText.isEmpty()) && keyEvent->key() != Qt::Key_Backspace) {
			return QWidget::eventFilter(obj, event);
		} else if (keyEvent->key() == Qt::Key_Backspace) {
            emit removeHumanEditor(m_pHumanEditor);
        }
		return true;
	}
	return QWidget::eventFilter(obj, event);
}

bool QFindDialog::eventFilter(QObject *obj, QEvent *event)
{
	if (obj == m_pHexEditor) {
		return eventHexEditor(obj, event);
	}
	
	if (obj == m_pHumanEditor) {
		return eventHumanEditor(obj, event);
	}
	return QWidget::eventFilter(obj, event);
}