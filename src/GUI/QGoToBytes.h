#ifndef QGOTOBYTES_H_
#define QGOTOBYTES_H_

#include <QDialog>
#include <QString>
#include <QLabel>
#include <QRadioButton>
#include <QLineEdit>
#include <QDialogButtonBox>

class QAbstractButton;

class QGoToBytes : public QDialog
{
	Q_OBJECT
public:
	QGoToBytes(QWidget * parent = 0);
	virtual ~QGoToBytes();

    QDialogButtonBox* getDialogButtonOk();

    int getRadioButton();

    QLineEdit* getLineOffset();

private:
    QLabel* m_pLabelType;
    QRadioButton* m_pRadioButtonHex;
    QRadioButton* m_pRadioButtonDec;
    QLabel* m_pLabelOffset;
    QLineEdit* m_pLineOffset;
    QDialogButtonBox* m_pButtonBox;

    int iRadioButton;

    void onRadioButtonClicked();
};

#endif /* QGOTOBYTES_H_ */
