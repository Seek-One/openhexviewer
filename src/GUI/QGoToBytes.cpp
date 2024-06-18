#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "QGoToBytes.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QIcon>
#include <QToolButton>
#include <QPushButton>

#include <QFile>


QGoToBytes::QGoToBytes(QWidget * parent)
{
    QVBoxLayout* pMainLayout = new QVBoxLayout();
    this->setLayout(pMainLayout);
    {
        QHBoxLayout* pHRadioLayout = new QHBoxLayout();
        pMainLayout->addLayout(pHRadioLayout);

        m_pLabelType = new QLabel("Offset type :");
        pHRadioLayout->addWidget(m_pLabelType);
        m_pRadioButtonHex = new QRadioButton("Hex", this);
        m_pRadioButtonHex->setChecked(true);
        pHRadioLayout->addWidget(m_pRadioButtonHex);
        m_pRadioButtonDec = new QRadioButton("Dec", this);
        pHRadioLayout->addWidget(m_pRadioButtonDec);
    }
    {
        QHBoxLayout* pHEditLayout = new QHBoxLayout();
        pMainLayout->addLayout(pHEditLayout);

        m_pLabelOffset = new QLabel("Offset :");
        pHEditLayout->addWidget(m_pLabelOffset);
        m_pLineOffset = new QLineEdit(this);
        pHEditLayout->addWidget(m_pLineOffset);
    }
    {
        QHBoxLayout* pHValidationLayout = new QHBoxLayout();
        pMainLayout->addLayout(pHValidationLayout);

        QDialogButtonBox* m_pButtonBox = new QDialogButtonBox(QDialogButtonBox::Cancel, Qt::Horizontal, this);
        pHValidationLayout->addWidget(m_pButtonBox);
        
        m_pButtonBox->addButton(QDialogButtonBox::Ok);

        connect(m_pButtonBox, SIGNAL(rejected()), this, SLOT(reject()));
        connect(m_pButtonBox, &QDialogButtonBox::accepted, [this]() {
            emit accepted();
        });
    }

    connect(m_pRadioButtonHex, &QRadioButton::clicked, this, &QGoToBytes::onRadioButtonClicked);
    connect(m_pRadioButtonDec, &QRadioButton::clicked, this, &QGoToBytes::onRadioButtonClicked);



    onRadioButtonClicked();
}

QGoToBytes::~QGoToBytes()
{

}

void QGoToBytes::onRadioButtonClicked() 
{
    if (m_pRadioButtonHex->isChecked()) {
        iRadioButton = 1;
    } else if (m_pRadioButtonDec->isChecked()) {
        iRadioButton = 2;
    }
}

QDialogButtonBox* QGoToBytes::getDialogButtonOk() 
{
    return m_pButtonBox;
}

int QGoToBytes::getRadioButton() 
{
    return iRadioButton;
}

QLineEdit* QGoToBytes::getLineOffset()
{
    return m_pLineOffset;
}