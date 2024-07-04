#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "QFileExportViewController.h"
#include "GUI/QFileExportView.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QIcon>
#include <QToolButton>
#include <QPushButton>
#include <QFileDialog>
#include <QTextStream>

#include <QFile>


QFileExportViewController::QFileExportViewController(QFileExportView* pFileExportView)
{
	m_pExportView = pFileExportView;
    m_iStart = 0;
    m_iSize = 0;
    m_iEnd = 0;

    connect(m_pExportView, SIGNAL(saveSelection()), this, SLOT(saveSelection()));
    connect(m_pExportView, SIGNAL(startOffsetChanged(const QString&)), this, SLOT(startOffsetChanged(const QString&)));
    connect(m_pExportView, SIGNAL(sizeOffsetChanged(const QString&)), this, SLOT(sizeOffsetChanged(const QString&)));
    connect(m_pExportView, SIGNAL(endOffsetChanged(const QString&)), this, SLOT(endOffsetChanged(const QString&)));
    connect(m_pExportView, SIGNAL(changeBase()), this, SLOT(changeBase()));
}

QFileExportViewController::~QFileExportViewController()
{

}

void QFileExportViewController::init(qint64 iStart, qint64 iSize)
{
    QSignalBlocker block(m_pExportView);
 
    m_iStart = iStart;
    m_iSize = iSize;
    m_iEnd = iStart + iSize;
    int iBase = m_pExportView->getBase();
    m_pExportView->setOffset(QString::number(m_iStart, iBase).toUpper(), QString::number(m_iSize, 10), QString::number(m_iEnd, iBase).toUpper());
}

void QFileExportViewController::startOffsetChanged(const QString &szText)
{
    QSignalBlocker block(m_pExportView);
    if (szText.isEmpty()) {
        return;
    }
    int iBase = m_pExportView->getBase();
    bool bOk;
    int iText = szText.toInt(&bOk, iBase);

    if (!bOk) {
        return;
    }
    m_iStart = iText;
    m_iSize = abs(m_iEnd - m_iStart);
    
    init(m_iStart, m_iSize);
    emit changeOffset(m_iStart, m_iSize);
}


void QFileExportViewController::sizeOffsetChanged(const QString &szText)
{
    QSignalBlocker block(m_pExportView);
    if (szText.isEmpty()) {
        return;
    }
    bool bOk;
    int iText = szText.toInt(&bOk, 10);

    if (!bOk) {
        return;
    }
    m_iSize = iText;
    m_iEnd = m_iStart + m_iSize;
    
    init(m_iStart, m_iSize);
    emit changeOffset(m_iStart, m_iSize);
}


void QFileExportViewController::endOffsetChanged(const QString &szText)
{
    QSignalBlocker block(m_pExportView);
    if (szText.isEmpty()) {
        return;
    }
    int iBase = m_pExportView->getBase();
    bool bOk;
    int iText = szText.toInt(&bOk, iBase);

    if (!bOk) {
        return;
    }
    m_iEnd = iText;
    m_iSize = abs(m_iEnd - m_iStart);

    init(m_iStart, m_iSize);
    emit changeOffset(m_iStart, m_iSize);
}

void QFileExportViewController::saveSelection()
{
    QString szText;
    emit selection(szText);

    QString filePath = QFileDialog::getSaveFileName(nullptr, "Save File", QDir::homePath());

    if (filePath.isEmpty()) {
        qWarning("[Export Selection] File path is empty");
	    QMessageBox::critical(this, tr("Problem"), tr("File pah is empty"), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning("[Export Selection] Write permission denied");
    	QMessageBox::critical(this, tr("Problem"), "Write permission denied", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    QTextStream out(&file);
    out << szText;
    file.close();

    qDebug("Selection saved to file");
	QMessageBox::critical(this, "", "Selection saved to file", QMessageBox::Ok, QMessageBox::Ok);
}

void QFileExportViewController::changeBase()
{
    init(m_iStart, m_iSize);
}