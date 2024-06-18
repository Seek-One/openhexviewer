#ifndef QGOTOBYTESCONTROLLER_H_
#define QGOTOBYTESCONTROLLER_H_

#include <QDialog>
#include <QString>

class QGoToBytes;
class QAbstractButton;

class QGoToBytesController : public QDialog
{
	Q_OBJECT
public:
	QGoToBytesController(QGoToBytes* pGoToBytes);
	virtual ~QGoToBytesController();

signals:
    void changeOffset(qint64 iOffset, qint64 size);

public slots:
    void researchOffSet();

private:
    QGoToBytes* m_pGoToBytes;
    
    void messageBox(QString szText);

};

#endif /* QGOTOBYTESCONTROLLER_H_ */
