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
#include <QPlainTextEdit>
#include <QScrollBar>

#include "QFileView.h"

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
    pMainLayout->addWidget(m_pOffsetEditor);

    m_pHexEditor = new QPlainTextEdit(pParent);
    font = m_pHexEditor->font();
    font.setFamily("DejaVu Sans Mono");
    m_pHexEditor->setFont(font);
    m_pHexEditor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pHexEditor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pHexEditor->setLineWrapMode(QPlainTextEdit::NoWrap);
    pMainLayout->addWidget(m_pHexEditor, 3);

    m_pHumanEditor = new QPlainTextEdit(pParent);
    font = m_pHumanEditor->font();
    font.setFamily("DejaVu Sans Mono");
    m_pHumanEditor->setFont(font);
    m_pHumanEditor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pHumanEditor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pHumanEditor->setLineWrapMode(QPlainTextEdit::NoWrap);
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

void QFileView::selectText(int iPosStart, int iPosEnd)
{
	QTextCursor c;

	c = m_pHumanEditor->textCursor();
	c.setPosition(iPosStart);
	c.setPosition(iPosEnd, QTextCursor::KeepAnchor);
	m_pHumanEditor->setTextCursor(c);

	c = m_pHexEditor->textCursor();
	c.setPosition(iPosStart*3);
	c.setPosition(iPosEnd*3-1, QTextCursor::KeepAnchor);
	m_pHexEditor->setTextCursor(c);
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
