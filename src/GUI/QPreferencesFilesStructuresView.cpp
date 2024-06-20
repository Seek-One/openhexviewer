#include <limits>
#include <math.h>

#include <QHBoxLayout>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QScrollBar>
#include <QStatusBar>
#include <QKeyEvent>
#include <QSignalBlocker>
#include <QObject>
#include <QLabel>

#include "QPreferencesFilesStructuresView.h"

QPreferencesFilesStructuresView::QPreferencesFilesStructuresView(QWidget* pParent)
	: QWidget(pParent)
{
    m_pFileTable = new QTableWidget(this);
}

QPreferencesFilesStructuresView::~QPreferencesFilesStructuresView()
{

}

QTableWidget* QPreferencesFilesStructuresView::getTableWidget() const
{
    return m_pFileTable;
}
