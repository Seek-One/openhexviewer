


#ifndef SRC_GUI_CONTROLLER_QBYTESVIEWCONTROLLER_H_
#define SRC_GUI_CONTROLLER_QBYTESVIEWCONTROLLER_H_

#include <QString>
#include <QObject>

#include "GUI/QBytesView.h"

class QBytesViewController : public QObject
{
    Q_OBJECT
public:
    QBytesViewController(QBytesView* pBytesView);
    virtual ~QBytesViewController();

public slots:
    void handleBytesChanged(QString szHex);

private:
    QBytesView* m_pBytesView;
    template<typename T>
    T convert(qint64 iValue);
    template<typename T>
    T swapEndian(T val);

    QString BeSignedInt(qint64 iValue, int iBits);
    QString BeUnsignedInt(qint64 iValue, int iBits);
    QString BeFloat(qint64 iValue, int iBits);
    QString BeHex(qint64 iValue, int iBits);
    QString BeOct(qint64 iValue, int iBits);
    QString BeBin(qint64 iValue, int iBits);

    QString LeSignedInt(qint64 iValue, int iBits);
    QString LeUnsignedInt(qint64 iValue, int iBits);
    QString LeFloat(qint64 iValue, int iBits);
    QString LeHex(qint64 iValue, int iBits);
    QString LeOct(qint64 iValue, int iBits);
    QString LeBin(qint64 iValue, int iBits);

	qint64 hexStringtoQInt64(QString szHex);
	QString removeSpace(QString szText);
};

#endif /* SRC_GUICONTROLLER_QBYTESVIEWCONTROLLER_H_ */