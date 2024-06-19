#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "QFindDialog.h"
#include "QFileView.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollBar>
#include <QPlainTextEdit>


QFindDialog::QFindDialog(QWidget * parent)
{
    QVBoxLayout* pMainLayout = new QVBoxLayout();
    setLayout(pMainLayout);

    {
        QBoxLayout* pViewLayout = new QHBoxLayout();
        pMainLayout->addLayout(pViewLayout);
        m_pFileView = new QFileView(this);
        pViewLayout->addWidget(m_pFileView);
    }

    {
        QHBoxLayout* pButtonLayout = new QHBoxLayout();
        pMainLayout->addLayout(pButtonLayout);
        
        m_pButtonFindPrevious = new QPushButton(tr("Find Previous"));
        pButtonLayout->addWidget(m_pButtonFindPrevious);
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

QFileView* getFileView()
{
    return m_pFileView;
}