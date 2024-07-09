


#ifndef SRC_GUI_CONTROLLER_QBYTESVIEWCONTROLLER_H_
#define SRC_GUI_CONTROLLER_QBYTESVIEWCONTROLLER_H_

#include <QString>
#include <QObject>

#include "GUI/QBytesView.h"

/**
 * @brief The controller class for handling byte data in the QBytesView.
 * 
 * The QBytesViewController class is responsible for managing byte data
 * changes and providing various conversions for byte data representations
 * in the QBytesView GUI component. It includes slots to handle byte data 
 * changes and utility functions for converting and formatting byte data 
 * in different representations such as signed/unsigned integers, floats, 
 * hexadecimal, octal, and binary formats.
 */
class QBytesViewController : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructs a QBytesViewController object.
     * @param pBytesView Pointer to the QBytesView object that this controller will manage.
     */
    QBytesViewController(QBytesView* pBytesView);

    /**
     * @brief Destroys the QBytesViewController object.
     */
    virtual ~QBytesViewController();

public slots:
    /**
     * @brief Handles changes in the byte data.
     * @param szHex The new byte data in hexadecimal format.
     */
    void handleBytesChanged(QString szHex);

private:
    QBytesView* m_pBytesView; /**< Pointer to the QBytesView object.*/

    /**
     * @brief Converts a value of type qint64 to a specified type T.
     * @tparam T The type to convert the value to.
     * @param iValue The value to be converted.
     * @return The converted value of type T.
     */
    template<typename T>
    T convert(qint64 iValue);

    /**
     * @brief Swaps the endianness of a value.
     * @tparam T The type of the value.
     * @param val The value whose endianness is to be swapped.
     * @return The value with swapped endianness.
     */
    template<typename T>
    T swapEndian(T val);

    /**
     * @brief Converts a big-endian signed integer.
     * @param iValue The integer value.
     * @param iBits The number of bits.
     * @return The string representation of the signed integer.
     */
    QString BeSignedInt(qint64 iValue, int iBits);

    /**
     * @brief Converts a big-endian unsigned integer.
     * @param iValue The integer value.
     * @param iBits The number of bits.
     * @return The string representation of the unsigned integer.
     */
    QString BeUnsignedInt(qint64 iValue, int iBits);

    /**
     * @brief Converts a big-endian float.
     * @param iValue The float value.
     * @param iBits The number of bits.
     * @return The string representation of the float.
     */
    QString BeFloat(qint64 iValue, int iBits);

    /**
     * @brief Converts a big-endian hexadecimal value.
     * @param iValue The value.
     * @param iBits The number of bits.
     * @return The string representation of the hexadecimal value.
     */
    QString BeHex(qint64 iValue, int iBits);

    /**
     * @brief Converts a big-endian octal value.
     * @param iValue The value.
     * @param iBits The number of bits.
     * @return The string representation of the octal value.
     */
    QString BeOct(qint64 iValue, int iBits);

    /**
     * @brief Converts a big-endian binary value.
     * @param iValue The value.
     * @param iBits The number of bits.
     * @return The string representation of the binary value.
     */
    QString BeBin(qint64 iValue, int iBits);

    /**
     * @brief Converts a little-endian signed integer.
     * @param iValue The integer value.
     * @param iBits The number of bits.
     * @return The string representation of the signed integer.
     */
    QString LeSignedInt(qint64 iValue, int iBits);

    /**
     * @brief Converts a little-endian unsigned integer.
     * @param iValue The integer value.
     * @param iBits The number of bits.
     * @return The string representation of the unsigned integer.
     */
    QString LeUnsignedInt(qint64 iValue, int iBits);

    /**
     * @brief Converts a little-endian float.
     * @param iValue The float value.
     * @param iBits The number of bits.
     * @return The string representation of the float.
     */
    QString LeFloat(qint64 iValue, int iBits);

    /**
     * @brief Converts a little-endian hexadecimal value.
     * @param iValue The value.
     * @param iBits The number of bits.
     * @return The string representation of the hexadecimal value.
     */
    QString LeHex(qint64 iValue, int iBits);

    /**
     * @brief Converts a little-endian octal value.
     * @param iValue The value.
     * @param iBits The number of bits.
     * @return The string representation of the octal value.
     */
    QString LeOct(qint64 iValue, int iBits);

    /**
     * @brief Converts a little-endian binary value.
     * @param iValue The value.
     * @param iBits The number of bits.
     * @return The string representation of the binary value.
     */
    QString LeBin(qint64 iValue, int iBits);

    /**
     * @brief Converts a hexadecimal string to a qint64 value.
     * @param szHex The hexadecimal string.
     * @return The converted qint64 value.
     */
    qint64 hexStringtoQInt64(QString szHex);

    /**
     * @brief Removes spaces from a given string.
     * @param szText The string from which spaces will be removed.
     * @return The string without spaces.
     */
    QString removeSpace(QString szText);
};

#endif /* SRC_GUICONTROLLER_QBYTESVIEWCONTROLLER_H_ */