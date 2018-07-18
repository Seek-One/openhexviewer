/*
 * QFileView.cpp
 *
 *  Created on: 18 juil. 2018
 *      Author: ebeuque
 */

#include <limits>
#include <math.h>

#include <QHBoxLayout>
#include <QTextEdit>
#include <QScrollBar>

#include "QFileView.h"

QFileView::QFileView(QWidget* pParent)
	: QWidget(pParent)
{
    QBoxLayout* pMainLayout = new QHBoxLayout();
    setLayout(pMainLayout);

    QFont font;

    m_pOffsetEditor = new QTextEdit(pParent);
    font = m_pOffsetEditor->font();
    font.setFamily("DejaVu Sans Mono");
    m_pOffsetEditor->setFont(font);
    m_pOffsetEditor->setFixedWidth(100);
    m_pOffsetEditor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pOffsetEditor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pOffsetEditor->setLineWrapMode(QTextEdit::FixedColumnWidth);
    m_pOffsetEditor->setLineWrapColumnOrWidth(10);
    pMainLayout->addWidget(m_pOffsetEditor);

    m_pHexEditor = new QTextEdit(pParent);
    font = m_pHexEditor->font();
    font.setFamily("DejaVu Sans Mono");
    m_pHexEditor->setFont(font);
    m_pHexEditor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pHexEditor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pHexEditor->setLineWrapMode(QTextEdit::FixedColumnWidth);
    m_pHexEditor->setAlignment(Qt::AlignCenter);
    pMainLayout->addWidget(m_pHexEditor, 3);

    m_pHumanEditor = new QTextEdit(pParent);
    font = m_pHumanEditor->font();
    font.setFamily("DejaVu Sans Mono");
    m_pHumanEditor->setFont(font);
    m_pHumanEditor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pHumanEditor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_pHumanEditor->setLineWrapMode(QTextEdit::FixedColumnWidth);
	m_pHumanEditor->setAlignment(Qt::AlignCenter);
    pMainLayout->addWidget(m_pHumanEditor, 1);

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
	int iWidth;
	iWidth = m_pHumanEditor->document()->size().width();;
	// Remove left and right margin
	iWidth -= m_pHumanEditor->document()->documentMargin();
	iWidth -= m_pHumanEditor->document()->documentMargin();
	QFontMetrics fontMetrics = m_pHumanEditor->fontMetrics();
	QRect rectChar = fontMetrics.boundingRect("\n");
	iWidth -= rectChar.width();

	rectChar = fontMetrics.boundingRect("0");

	int iResult = qRound(iWidth/(float)(rectChar.width()));

	m_pHexEditor->setLineWrapColumnOrWidth(iResult*3);
	m_pHumanEditor->setLineWrapColumnOrWidth(iResult);

	return iResult;
}

int QFileView::getVisibleRowCount() const
{
	int iHeight;
	iHeight = m_pHumanEditor->height();
	// Remove top and bottom margin
	iHeight -= m_pHumanEditor->document()->documentMargin();
	iHeight -= m_pHumanEditor->document()->documentMargin();
	QFontMetrics fontMetrics = m_pHumanEditor->fontMetrics();
	QRect rectChar = fontMetrics.boundingRect("0");
	return (int)floor(iHeight/(float)rectChar.height());
}

void QFileView::setOffsetText(const QString& szText)
{
	m_pOffsetEditor->setText(szText);
}

void QFileView::setHexText(const QString& szText)
{
	m_pHexEditor->setText(szText);
}

void QFileView::setHumanText(const QString& szText)
{
	m_pHumanEditor->setText(szText);
}

void QFileView::resizeEvent(QResizeEvent *event)
{
	emit sizeChanged();
}
