#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <limits>
#include <math.h>

#include "QFindDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QRadioButton>
#include <QButtonGroup>

QFindDialog::QFindDialog(QWidget * pParent)
{
    QVBoxLayout* pMainLayout = new QVBoxLayout();
    setLayout(pMainLayout);
    {
        QBoxLayout* pOffsetLayout = new QHBoxLayout();
        pMainLayout->addLayout(pOffsetLayout);
        {
            createSettings(pOffsetLayout);
        }
    }
    {
        QBoxLayout* pViewLayout = new QHBoxLayout();
        pMainLayout->addLayout(pViewLayout);
        {
            createView(pViewLayout);
        }   
    }
    {
        QHBoxLayout* pButtonLayout = new QHBoxLayout();
        pMainLayout->addLayout(pButtonLayout);
        {
            createButton(pButtonLayout);
        }
    }
}

QFindDialog::~QFindDialog()
{

}

void QFindDialog::createSettings(QBoxLayout* pParentLayout)
{
#ifdef USE_NO_QREGEXP
    QRegularExpression hexRegex("[0-9A-Fa-f]*");
    QRegularExpressionValidator *hexValidator = new QRegularExpressionValidator(hexRegex, this);

    QRegularExpression decRegex("[0-9]*");
    QRegularExpressionValidator *decValidator = new QRegularExpressionValidator(decRegex, this);
#else
    QRegExp hexRegex("[0-9A-Fa-f]*");
    QRegExpValidator *hexValidator = new QRegExpValidator(hexRegex, this);

    QRegExp decRegex("[0-9]*");
    QRegExpValidator *decValidator = new QRegExpValidator(decRegex, this);
#endif   
    QGroupBox* pFindGroupBox = new QGroupBox(tr("Research settings"));
    pParentLayout->addWidget(pFindGroupBox);
    
    
    QGridLayout* pGridLayout = new QGridLayout();    
    pFindGroupBox->setLayout(pGridLayout);
    
    {
        QLabel* pStartLabel = new QLabel(tr("Start :"));
        pStartLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        pGridLayout->addWidget(pStartLabel, 0, 0);
        
        QButtonGroup* pStartButton = new QButtonGroup();
        
        QRadioButton* pFileStartButton = new QRadioButton(tr("Start of file"));
        pFileStartButton->setChecked(true);
        pGridLayout->addWidget(pFileStartButton, 0, 1);
        pStartButton->addButton(pFileStartButton);        
    
        QWidget* pOffsetWidget = new QWidget();
        pGridLayout->addWidget(pOffsetWidget, 0, 2);

        QBoxLayout* pStartOffsetLayout = new QHBoxLayout();
        pOffsetWidget->setLayout(pStartOffsetLayout);

        QRadioButton* pOffsetStartButton = new QRadioButton(tr("Offset :"));
        pStartOffsetLayout->addWidget(pOffsetStartButton);
        pStartButton->addButton(pOffsetStartButton);

        m_pStartEdit = new QLineEdit();
        m_pStartEdit->setEnabled(false);
        m_pStartEdit->setValidator(hexValidator);
        pStartOffsetLayout->addWidget(m_pStartEdit);

        QComboBox* pStartCombo = new QComboBox();
        pStartCombo->addItem("HEX");
        pStartCombo->addItem("DEC");
        pStartCombo->setEnabled(false);
        pStartOffsetLayout->addWidget(pStartCombo);
        
        connect(pFileStartButton, &QRadioButton::toggled, [this]() {
            m_offsets.startOffset.type = NORMAL;
        });
        connect(pOffsetStartButton, &QRadioButton::toggled, [this, pStartCombo]() {
            m_offsets.startOffset.type = CUSTOM;
            m_offsets.startOffset.base = 16;
            pStartCombo->setCurrentText("HEX");
        });
        connect(pStartCombo, QOverload<const QString &>::of(&QComboBox::currentTextChanged), [this, hexValidator, decValidator](QString szType) {
            bool bOk;
            QString szText = m_pStartEdit->text();
            int iText = szText.toInt(&bOk, m_offsets.startOffset.base);
            if (!bOk) {
                return;
            }
            if (szType == "HEX") {
                m_pStartEdit->setValidator(hexValidator);
                m_offsets.startOffset.base = 16;
                m_pStartEdit->setText(QString::number(iText, 16).toUpper());
            } else if (szType == "DEC") {
                m_pStartEdit->setValidator(decValidator);
                m_offsets.startOffset.base = 10;
                m_pStartEdit->setText(QString::number(iText, 10).toUpper());
            }
            emit offsetChanged();
        });

        connect(m_pStartEdit, &QLineEdit::textChanged, [this]() {
            emit offsetChanged();
        });

        connect(pOffsetStartButton, &QRadioButton::toggled, this, [this, pStartCombo](bool checked){
            m_pStartEdit->setEnabled(checked);
            pStartCombo->setEnabled(checked);
            if (!checked) {
                m_pStartEdit->clear(); 
            } else {
                m_pStartEdit->setFocus();
            }
        });
    }
    {
        QLabel* pEndLabel = new QLabel(tr("End :"));
        pEndLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        pGridLayout->addWidget(pEndLabel, 1, 0);

        QButtonGroup* pEndButton = new QButtonGroup();

        QRadioButton* pFileEndButton = new QRadioButton(tr("End of file"));
        pFileEndButton->setChecked(true);
        pGridLayout->addWidget(pFileEndButton, 1, 1);
        pEndButton->addButton(pFileEndButton);

        QWidget* pOffsetWidget = new QWidget();
        pGridLayout->addWidget(pOffsetWidget, 1, 2);

        QBoxLayout* pEndOffsetLayout = new QHBoxLayout();    
        pOffsetWidget->setLayout(pEndOffsetLayout);

        QRadioButton* pOffsetEndButton = new QRadioButton(tr("Offset :"));
        pEndOffsetLayout->addWidget(pOffsetEndButton);
        pEndButton->addButton(pOffsetEndButton);

        m_pEndEdit = new QLineEdit();
        m_pEndEdit->setEnabled(false);
        m_pEndEdit->setValidator(hexValidator);
        pEndOffsetLayout->addWidget(m_pEndEdit);

        QComboBox* pEndCombo = new QComboBox();
        pEndCombo->addItem("HEX");
        pEndCombo->addItem("DEC");
        pEndCombo->setEnabled(false);
            pEndOffsetLayout->addWidget(pEndCombo);

        connect(pFileEndButton, &QRadioButton::toggled, [this]() {
            m_offsets.endOffset.type = NORMAL;
        });
        connect(pOffsetEndButton, &QRadioButton::toggled, [this, pEndCombo]() {
            m_offsets.endOffset.type = CUSTOM;
            m_offsets.endOffset.base = 16;
            pEndCombo->setCurrentText("HEX");
        });
        connect(pEndCombo, QOverload<const QString &>::of(&QComboBox::currentTextChanged), [this, hexValidator, decValidator](QString szType) {
            bool bOk;
            QString szText = m_pEndEdit->text();
            int iText = szText.toInt(&bOk, m_offsets.endOffset.base);

            if (!bOk) {
                return;
            }
            if (szType == "HEX") {
                m_pEndEdit->setValidator(hexValidator);
                m_offsets.endOffset.base = 16;
                m_pEndEdit->setText(QString::number(iText, 16).toUpper());
            } else if (szType == "DEC") {
                m_pEndEdit->setValidator(decValidator);
                m_offsets.endOffset.base = 10;
                m_pEndEdit->setText(QString::number(iText, 10).toUpper());                        
            }
            emit offsetChanged();
        });

        connect(m_pEndEdit, &QLineEdit::textChanged, [this]() {
            emit offsetChanged();
        });

        connect(pOffsetEndButton, &QRadioButton::toggled, this, [this, pEndCombo](bool checked){
            m_pEndEdit->setEnabled(checked);
            pEndCombo->setEnabled(checked);
            if (!checked) {
                m_pEndEdit->clear(); 
            } else {
                m_pEndEdit->setFocus();
            }
        });
        
    }
}

void QFindDialog::createView(QBoxLayout* pParentLayout)
{
    QFont font;

    m_pHexEditor = new QPlainTextEdit();
    font = m_pHexEditor->font();
    font.setFamily("DejaVu Sans Mono");
    m_pHexEditor->setFont(font);
    m_pHexEditor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pHexEditor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pHexEditor->setLineWrapMode(QPlainTextEdit::NoWrap);
    m_pHexEditor->installEventFilter(this);
    pParentLayout->addWidget(m_pHexEditor, 3);

    m_pHumanEditor = new QPlainTextEdit();
    font = m_pHumanEditor->font();
    font.setFamily("DejaVu Sans Mono");
    m_pHumanEditor->setFont(font);
    m_pHumanEditor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pHumanEditor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pHumanEditor->setLineWrapMode(QPlainTextEdit::NoWrap);
    m_pHumanEditor->installEventFilter(this);
    pParentLayout->addWidget(m_pHumanEditor, 1);

    m_pScrollBar = new QScrollBar();
    pParentLayout->addWidget(m_pScrollBar);

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

void QFindDialog::createButton(QBoxLayout* pParentLayout)
{
    m_pButtonFindPrevious = new QPushButton(tr("Find Previous"));
    pParentLayout->addWidget(m_pButtonFindPrevious);

    m_pLabelNbOcc = new QLabel("");
    pParentLayout->addWidget(m_pLabelNbOcc);
    m_pComboPosOcc = new QComboBox();
    m_pComboPosOcc->setHidden(true);
    pParentLayout->addWidget(m_pComboPosOcc);

    m_pButtonFindNext = new QPushButton(tr("Find Next"));
    pParentLayout->addWidget(m_pButtonFindNext);
    m_pButtonBox = new QDialogButtonBox(QDialogButtonBox::Cancel);
    pParentLayout->addWidget(m_pButtonBox);

    connect(m_pComboPosOcc, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int iIndex) {
        emit comboOffsetChanged(iIndex);
    });

    connect(m_pButtonBox, SIGNAL(rejected()), this, SLOT(reject()));

    connect(m_pButtonFindNext, &QAbstractButton::clicked, [this]() {
        emit findNext();
    });

    connect(m_pButtonFindPrevious, &QAbstractButton::clicked, [this]() {
        emit findPrevious();
    });
}

OffsetValue QFindDialog::getOffsets()
{
    return m_offsets;
}

QString QFindDialog::getStartOffset()
{
    return m_pStartEdit->text();
}

QString QFindDialog::getEndOffset()
{
    return m_pEndEdit->text();
}

QComboBox* QFindDialog::getComboPosOcc()
{
    return m_pComboPosOcc;
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
        m_pComboPosOcc->setHidden(true);
    } else {
        m_pLabelNbOcc->setText(QString(tr("%0/%1")).arg(iIndexOcc).arg(iNbOcc));
        m_pComboPosOcc->setCurrentIndex(iIndexOcc - 1);
        m_pComboPosOcc->setHidden(false);
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
		if (!keyText.isEmpty() && keyText.at(0).unicode() >= 0x20 && keyText.at(0).unicode() <= 0x7E) {
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
