#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <limits>
#include <math.h>

#include "QFindDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
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
        m_pButtonBox = new QDialogButtonBox(QDialogButtonBox::Cancel);
        pButtonLayout->addWidget(m_pButtonBox);

        connect(m_pButtonBox, SIGNAL(rejected()), this, SLOT(reject()));

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
		
		bool isMovingKey = (keyEvent->key() == Qt::Key_Left ||
							keyEvent->key() == Qt::Key_Right ||
							keyEvent->key() == Qt::Key_Up ||
							keyEvent->key() == Qt::Key_Down);
		
		bool isTextKey = allowedChars.contains(keyText);
		bool isDigitKey = keyEvent->key() >= Qt::Key_0 && keyEvent->key() <= Qt::Key_9;

		if (isMovingKey) {
            return QWidget::eventFilter(obj, event);
        } else if (keyEvent->key() == Qt::Key_Backspace || keyEvent->key() == Qt::Key_Delete) {
            emit removeHexEditor(m_pHexEditor);
        } else if ((!keyText.isEmpty() && isTextKey && (keyEvent->modifiers() == Qt::NoModifier || keyEvent->modifiers() == Qt::ShiftModifier)) || isDigitKey) {
            emit insertCharHexEditor(m_pHexEditor, keyText);
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
		if (!keyText.isEmpty() && keyText.at(0) >= 0x20 && keyText.at(0) <= 0x7E) {
            emit insertCharHumanEditor(m_pHumanEditor, keyText);
            return true;
        } else if (keyEvent->key() == Qt::Key_Backspace || keyEvent->key() == Qt::Key_Delete) {
            emit removeHumanEditor(m_pHumanEditor);
            return true;
        }
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