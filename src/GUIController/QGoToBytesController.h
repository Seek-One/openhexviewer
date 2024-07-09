#ifndef QGOTOBYTESCONTROLLER_H_
#define QGOTOBYTESCONTROLLER_H_

#include <QDialog>
#include <QString>

class QGoToBytes;
class QAbstractButton;

/**
 * @brief The controller class for the "Go To Bytes" dialog.
 * 
 * The QGoToBytesController class manages the interactions between the QGoToBytes GUI component 
 * and the application logic for navigating to specific byte offsets within a file. It provides
 * functionalities to initiate a search for a specific byte offset and emit signals to notify other
 * components of changes.
 */
class QGoToBytesController : public QDialog
{
	Q_OBJECT
public:
    /**
     * @brief Constructs a QGoToBytesController object.
     * @param pGoToBytes Pointer to the QGoToBytes object that this controller will manage.
     */
	QGoToBytesController(QGoToBytes* pGoToBytes);

    /**
     * @brief Destroys the QGoToBytesController object.
     */
	virtual ~QGoToBytesController();

signals:
    /**
     * @brief Signal emitted to change the offset within the file.
     * @param iOffset The new offset to navigate to.
     * @param size The size of the data at the new offset.
     */
    void changeOffset(qint64 iOffset, qint64 size);

public slots:
    /**
     * @brief Initiates a search for the specified byte offset.
     */
    void researchOffSet();

private:
    QGoToBytes* m_pGoToBytes; /**< Pointer to the QGoToBytes object. */
};

#endif /* QGOTOBYTESCONTROLLER_H_ */
