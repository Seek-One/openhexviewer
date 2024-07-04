#ifndef QGOTOBYTES_H_
#define QGOTOBYTES_H_

#include <QDialog>
#include <QLineEdit>

class QAbstractButton;

class QGoToBytes : public QDialog
{
	Q_OBJECT
public:
	QGoToBytes(QWidget * parent = 0);
	virtual ~QGoToBytes();

    int getLineBase();

    QString getLineOffset();

private:
    QLineEdit* m_pLineOffset;

    int m_iBase;
};

#endif /* QGOTOBYTES_H_ */
