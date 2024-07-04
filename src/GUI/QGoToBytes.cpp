#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "QGoToBytes.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QRadioButton>

QGoToBytes::QGoToBytes(QWidget * parent)
{
    QVBoxLayout* pMainLayout = new QVBoxLayout();
    this->setLayout(pMainLayout);
    {
        QHBoxLayout* pHRadioLayout = new QHBoxLayout();
        pMainLayout->addLayout(pHRadioLayout);

        QLabel* pLabelType = new QLabel(tr("Offset type :"));
        pHRadioLayout->addWidget(pLabelType);
        QRadioButton* pRadioButtonHex = new QRadioButton(tr("Hex"), this);
        pRadioButtonHex->setChecked(true);
        connect(pRadioButtonHex, &QRadioButton::clicked, [this]() {
            m_iBase = 16;
        });
        pHRadioLayout->addWidget(pRadioButtonHex);
        QRadioButton* pRadioButtonDec = new QRadioButton(tr("Dec"), this);
        connect(pRadioButtonDec, &QRadioButton::clicked, [this]() {
            m_iBase = 10;
        });
        pHRadioLayout->addWidget(pRadioButtonDec);
    }
    {
        QHBoxLayout* pHEditLayout = new QHBoxLayout();
        pMainLayout->addLayout(pHEditLayout);

        QLabel* pLabelOffset = new QLabel(tr("Offset :"));
        pHEditLayout->addWidget(pLabelOffset);
        m_pLineOffset = new QLineEdit(this);
        pHEditLayout->addWidget(m_pLineOffset);
    }
    {
        QHBoxLayout* pHValidationLayout = new QHBoxLayout();
        pMainLayout->addLayout(pHValidationLayout);

        QDialogButtonBox* pButtonBox = new QDialogButtonBox(QDialogButtonBox::Cancel, Qt::Horizontal, this);
        pHValidationLayout->addWidget(pButtonBox);
        
        pButtonBox->addButton(QDialogButtonBox::Ok);

        connect(pButtonBox, SIGNAL(rejected()), this, SLOT(reject()));
        connect(pButtonBox, &QDialogButtonBox::accepted, [this]() {
            emit accepted();
        });
    }
}

QGoToBytes::~QGoToBytes()
{

}

int QGoToBytes::getLineBase() 
{
    return m_iBase;
}

QString QGoToBytes::getLineOffset()
{
    return m_pLineOffset->text();
}