


#include "Global/QtCompat.h"
#include "GUI/QBytesView.h"

#include <QString>

#include "QBytesViewController.h"

#include <QDebug>

QBytesViewController::QBytesViewController(QBytesView* pBytesView)
{
    m_pBytesView = pBytesView;
}

QBytesViewController::~QBytesViewController() 
{
}

void QBytesViewController::handleBytesChanged(QString szHex) 
{
    szHex = removeSpace(szHex);
    qint64 iNb = hexStringtoQInt64(szHex);
    int iBits = szHex.length() * 4;

    if (iBits <= 64) {
        m_pBytesView->showTable();

        //BE
        m_pBytesView->renameRowHeaders(0, QString("Signed Integer (%1 bits)").arg(iBits));
        m_pBytesView->setItem(0, BeSignedInt(iNb, iBits));
        m_pBytesView->renameRowHeaders(1, QString("Unsigned Integer (%1 bits)").arg(iBits));
        m_pBytesView->setItem(1, BeUnsignedInt(iNb, iBits));
        m_pBytesView->renameRowHeaders(2, QString("Float (%1 bits)").arg(iBits));
        m_pBytesView->setItem(2, BeFloat(iNb, iBits));
        m_pBytesView->setItem(3, BeHex(iNb, iBits));
        m_pBytesView->setItem(4, BeOct(iNb, iBits));
        m_pBytesView->setItem(5, BeBin(iNb, iBits));

        //LE
        m_pBytesView->renameRowHeaders(6, QString("Signed Integer (%1 bits)").arg(iBits));
        m_pBytesView->setItem(6, LeSignedInt(iNb, iBits));
        m_pBytesView->renameRowHeaders(7, QString("Unsigned Integer (%1 bits)").arg(iBits));
        m_pBytesView->setItem(7, LeUnsignedInt(iNb, iBits));
        m_pBytesView->renameRowHeaders(8, QString("Float (%1 bits)").arg(iBits));
        m_pBytesView->setItem(8, LeFloat(iNb, iBits));
        m_pBytesView->setItem(9, LeHex(iNb, iBits));
        m_pBytesView->setItem(10, LeOct(iNb, iBits));
        m_pBytesView->setItem(11, LeBin(iNb, iBits));

        m_pBytesView->resizeColumnsToContents();
    } else {
        m_pBytesView->showWarning();
    }
}

QString removeTrailingZeros(QString szText)
{
    while (szText.endsWith('0')) {
        szText.chop(1);
    }
    return szText;
}

template<typename T>
T QBytesViewController::swapEndian(T val)
{
    T swapped;
    auto *pVal = reinterpret_cast<uint8_t *>(&val);
    auto *pSwapped = reinterpret_cast<uint8_t *>(&swapped);
    for (size_t i = 0; i < sizeof(T); i++) {
        pSwapped[i] = pVal[sizeof(T) - 1 - i];
    }
    return swapped;
}

template<typename T>
T QBytesViewController::convert(qint64 iValue)
{
    return static_cast<T>(iValue);  
}

QString QBytesViewController::BeSignedInt(qint64 iValue, int iBits)
{
    if (iBits <= 8) {
        return QString::number(convert<int8_t>(iValue));
    } 
    if (iBits <= 16) {
        return QString::number(convert<int16_t>(iValue));
    } 
    if (iBits <= 32) {
        return QString::number(convert<int32_t>(iValue));
    } 
    if (iBits <= 64) {
        return QString::number(convert<int64_t>(iValue));
    } 
    return "0";
    
}

QString QBytesViewController::BeUnsignedInt(qint64 iValue, int iBits)
{
    if (iBits <= 8) {
        return QString::number(convert<uint8_t>(iValue));
    } 
    if (iBits <= 16) {
        return QString::number(convert<uint16_t>(iValue));
    } 
    if (iBits <= 32) {
        return QString::number(convert<uint32_t>(iValue));
    } 
    if (iBits <= 64) {
        return QString::number(convert<uint64_t>(iValue));
    } 
    return "0";
}

QString QBytesViewController::BeFloat(qint64 iValue, int iBits)
{
    if (iBits <= 32) {
        return QString::number(reinterpret_cast<float&>(iValue));
        
    } 
    if (iBits <= 64) {
        return QString::number(reinterpret_cast<double&>(iValue));
    }
    return "0";
}

QString QBytesViewController::BeHex(qint64 iValue, int iBits)
{
    return QString("%1").arg(iValue, 0, 16).toUpper();
}

QString QBytesViewController::BeOct(qint64 iValue, int iBits)
{
    return QString("%1").arg(iValue, 0, 8);
}

QString QBytesViewController::BeBin(qint64 iValue, int iBits)
{
    return QString("%1").arg(iValue, 0, 2);
}

QString QBytesViewController::LeSignedInt(qint64 iValue, int iBits)
{
    return BeSignedInt(swapEndian(iValue << (64 - iBits)), iBits);
}

QString QBytesViewController::LeUnsignedInt(qint64 iValue, int iBits)
{
    return BeUnsignedInt(swapEndian(iValue << (64 - iBits)), iBits);
}

QString QBytesViewController::LeFloat(qint64 iValue, int iBits)
{
    // return QString::number(swapEndian(iValue << 32));
    return BeFloat(swapEndian(iValue << (64 - iBits)), iBits);
}

QString QBytesViewController::LeHex(qint64 iValue, int iBits)
{
    QString szRes = BeHex(swapEndian(iValue), iBits);
    if (szRes.length() > 1) {
        return removeTrailingZeros(szRes);
    }
    return "0";
}

QString QBytesViewController::LeOct(qint64 iValue, int iBits)
{
    QString szRes = BeOct(swapEndian(iValue), iBits);
    if (szRes.length() > 1) {
        return removeTrailingZeros(szRes);
    }
    return "0";
}

QString QBytesViewController::LeBin(qint64 iValue, int iBits)
{
    QString szRes = BeBin(swapEndian(iValue), iBits);
    if (szRes.length() > 1) {
        return removeTrailingZeros(szRes);
    }
    return "0";
}

qint64 QBytesViewController::hexStringtoQInt64(QString szHex) 
{
	if (szHex.isEmpty()) {
		qDebug("[QBytesViewController] Empty string cannot be converted to qint64");
		return 0;
	}

	QRegExp hexMatcher("^[0-9A-Fa-f]+$");
	if (!hexMatcher.exactMatch(szHex)) {
		qDebug("[QBytesViewController] Invalid characters in hex string");
		return 0;
	}
	
	bool ok;
    if (szHex.length() > 16) {
        return 0;
    }
	qint64 iRes = szHex.toLongLong(&ok, 16);
	if (!ok) {
		qDebug("[QBytesViewController] Conversion failed");
		return 0;
	}
	return iRes;
}

QString QBytesViewController::removeSpace(QString szText) 
{
	QString szRes = szText;
	szRes.remove(' ');
	szRes.remove('\n');
	return szRes;
}