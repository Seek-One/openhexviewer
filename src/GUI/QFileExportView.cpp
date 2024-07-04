#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <limits>
#include <math.h>

#include "QFileExportView.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QGroupBox>

QFileExportView::QFileExportView(QWidget * pParent)
{  
    QRegularExpression hexRegex("[0-9A-Fa-f]*");
    QRegularExpressionValidator *hexValidator = new QRegularExpressionValidator(hexRegex, this);

    QRegularExpression decRegex("[0-9]*");
    QRegularExpressionValidator *decValidator = new QRegularExpressionValidator(decRegex, this);

    QVBoxLayout* pMainLayout = new QVBoxLayout(this);
    setLayout(pMainLayout);
    {
        QGroupBox* pOffsetBox = new QGroupBox(tr("Offset"));
        pMainLayout->addWidget(pOffsetBox);
        {
            QGridLayout* pGroupGridLayout = new QGridLayout();
            pOffsetBox->setLayout(pGroupGridLayout);
            {
                {
                    QLabel* pLabelType = new QLabel(tr("Type :"));
                    pLabelType->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
                    pGroupGridLayout->addWidget(pLabelType, 0, 0);

                    QWidget* pWdiget = new QWidget();
                    pGroupGridLayout->addWidget(pWdiget, 0, 1);
                    QHBoxLayout* pHRadioLayout = new QHBoxLayout();
                    pWdiget->setLayout(pHRadioLayout);
                    {
                        QRadioButton* pRadioButtonHex = new QRadioButton(tr("Hex"));
                        pRadioButtonHex->setChecked(true);
                        connect(pRadioButtonHex, &QRadioButton::clicked, [this, hexValidator] () {
                            m_iRadioButton = 16;
                            m_pStartOffset->setValidator(hexValidator);
                            m_pEndOffset->setValidator(hexValidator);
                            emit changeBase();
                        });
                        pHRadioLayout->addWidget(pRadioButtonHex);
                        
                        QRadioButton* pRadioButtonDec = new QRadioButton(tr("Dec"));
                        pHRadioLayout->addWidget(pRadioButtonDec);
                        connect(pRadioButtonDec, &QRadioButton::clicked, [this, decValidator] () {
                            m_iRadioButton = 10;
                            m_pStartOffset->setValidator(decValidator);
                            m_pEndOffset->setValidator(decValidator);
                            emit changeBase();
                        });
                    }
                }
                QLabel* pStartOffset = new QLabel(tr("Start :"));
                pStartOffset->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
                pGroupGridLayout->addWidget(pStartOffset, 1, 0);
                m_pStartOffset = new QLineEdit();
                connect(m_pStartOffset, &QLineEdit::textChanged, [this](const QString &szText) {
                    emit startOffsetChanged(szText);
                });
                m_pStartOffset->setValidator(hexValidator);
                pGroupGridLayout->addWidget(m_pStartOffset, 1, 1);
            
                QLabel* pEndOffset = new QLabel(tr("End :"));
                pEndOffset->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
                pGroupGridLayout->addWidget(pEndOffset, 2, 0);
                m_pEndOffset = new QLineEdit();
                connect(m_pEndOffset, &QLineEdit::textChanged, [this](const QString &szText) {
                    emit endOffsetChanged(szText);
                });
                m_pEndOffset->setValidator(hexValidator);
                pGroupGridLayout->addWidget(m_pEndOffset, 2, 1);
            }
        }

        QGroupBox* pSizeBox = new QGroupBox(tr("Size"));
        pMainLayout->addWidget(pSizeBox);
        {
            QGridLayout* pGroupGridLayout = new QGridLayout();
            pSizeBox->setLayout(pGroupGridLayout);
            {
                QLabel* pSizeOffset = new QLabel(tr("Size :"));
                pSizeOffset->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
                pGroupGridLayout->addWidget(pSizeOffset, 0, 0);
                m_pSizeOffset = new QLineEdit();
                connect(m_pSizeOffset, &QLineEdit::textChanged, [this](const QString &szText) {
                    emit endOffsetChanged(szText);
                });
                m_pSizeOffset->setValidator(decValidator);
                pGroupGridLayout->addWidget(m_pSizeOffset, 0, 1);
            }
        }
    }

    {
        QHBoxLayout* pHValidationLayout = new QHBoxLayout();
        pMainLayout->addLayout(pHValidationLayout);

        QDialogButtonBox* pButtonBox = new QDialogButtonBox(QDialogButtonBox::Cancel, Qt::Horizontal, this);
        pHValidationLayout->addWidget(pButtonBox);
        
        pButtonBox->addButton(QDialogButtonBox::Save);

        connect(pButtonBox, SIGNAL(rejected()), this, SLOT(reject()));
        connect(pButtonBox, &QDialogButtonBox::accepted, [this]() {
            emit saveSelection();
        });
    }
}

QFileExportView::~QFileExportView()
{
}

int QFileExportView::getBase() 
{
    return m_iRadioButton;
}

void QFileExportView::setOffset(QString szStart, QString szSize, QString szEnd)
{
    QSignalBlocker blocker(this);
    m_pStartOffset->setText(szStart);
    m_pSizeOffset->setText(szSize);
    m_pEndOffset->setText(szEnd);
}