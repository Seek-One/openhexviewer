
#include "QBytesView.h"

#include <QStandardItem>
#include <QHeaderView>
#include <QVBoxLayout>

QBytesView::QBytesView(QWidget* pParent)
    : QWidget(pParent)
{ 
    m_pTableView = new QTableView(this);
    m_pWarningLabel = new QLabel(tr("Cannot display data representation for more than 64 bits"), this);

    m_pModel = new QStandardItemModel(12, 3, this);

    for (int iRow = 0; iRow < 12; ++iRow) {
        for (int iColumn = 0; iColumn < 3; ++iColumn) {
            QStandardItem *pItem = new QStandardItem(" ");
            m_pModel->setItem(iRow, iColumn, pItem);
        }
    }

    for (int iRow = 0; iRow < 12; ++iRow) {
        if (iRow < 6) {
            setItem(iRow, 0, tr("BigEndian"));
        } else {
            setItem(iRow, 0, tr("LittleEndian"));
        }
    }
    setItem(0, 1, tr("Signed Integer"));
    setItem(1, 1, tr("Unsigned Integer"));
    setItem(2, 1, tr("Float"));
    setItem(3, 1, tr("Hexadecimal"));
    setItem(4, 1, tr("Octal"));
    setItem(5, 1, tr("Binary"));
    setItem(6, 1, tr("Signed Integer"));
    setItem(7, 1, tr("Unsigned Integer"));
    setItem(8, 1, tr("Float"));
    setItem(9, 1, tr("Hexadecimal"));
    setItem(10, 1, tr("Octal"));
    setItem(11, 1, tr("Binary"));

    // > 64 bits Not possible to display le contenu de la selection

    m_pTableView->setModel(m_pModel);
    m_pTableView->resizeColumnsToContents();
    m_pTableView->horizontalHeader()->setVisible(false);
    m_pTableView->verticalHeader()->setVisible(false);

    m_pWarningLabel->setAlignment(Qt::AlignCenter);
    m_pWarningLabel->setStyleSheet("QLabel { color : red;}");
    m_pWarningLabel->hide();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_pTableView);
    layout->addWidget(m_pWarningLabel);

    // Set the layout on the main window
    this->setLayout(layout);

    // Ensure the table view and warning label expand to fill the layout
    m_pTableView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pWarningLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}


void QBytesView::setItem(int iRow, QString szText) {
    m_pModel->item(iRow, 2)->setText(szText);
}

void QBytesView::renameRowHeaders(int iRow, QString szText) {
    m_pModel->item(iRow, 1)->setText(szText);
}

void QBytesView::setItem(int iRow, int iColumn, QString szText) {
    m_pModel->item(iRow, iColumn)->setText(szText);
}

void QBytesView::resizeColumnsToContents() {
    m_pTableView->resizeColumnsToContents();
}

void QBytesView::showWarning() {
    m_pTableView->hide();
    m_pWarningLabel->show();
}

void QBytesView::showTable() {
    m_pTableView->show();
    m_pWarningLabel->hide();
}

