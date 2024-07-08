#ifndef QGOTOBYTES_H_
#define QGOTOBYTES_H_

#include <QDialog>
#include <QLineEdit>

class QAbstractButton;

/**
 * @brief A dialog for navigating to a specific byte offset in a file.
 * 
 * The QGoToBytes class provides a dialog interface for inputting a byte
 * offset and base to navigate to within a file. It includes methods for
 * retrieving the base and offset values.
 */
class QGoToBytes : public QDialog
{
	Q_OBJECT
public:
    /**
     * @brief Constructs a new QGoToBytes object.
     * 
     * @param parent The parent widget of the QGoToBytes dialog. Defaults to nullptr.
     */
	QGoToBytes(QWidget * parent = 0);

    /**
     * @brief Destroys the QGoToBytes object.
     */
	virtual ~QGoToBytes();

    /**
     * @brief Gets the line base value.
     * 
     * @return The line base value as an integer.
     */
    int getLineBase();

    /**
     * @brief Gets the line offset value.
     * 
     * @return The line offset value as a QString.
     */
    QString getLineOffset();

private:
    QLineEdit* m_pLineOffset; /**< Line edit for entering the byte offset. */
    int m_iBase; /**< The base value for the offset (e.g., 10 for decimal, 16 for hexadecimal). */
};

#endif /* QGOTOBYTES_H_ */
