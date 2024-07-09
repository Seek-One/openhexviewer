#ifndef QFINDDIALOGCONTROLLER_H_
#define QFINDDIALOGCONTROLLER_H_

#include <QDialog>
#include <QByteArray>

#include "GUI/QFindDialog.h"
#include "QFileViewController.h"

class QFindDialog;
class QPlainTextEdit;

/**
 * @brief The controller class for the find dialog.
 * 
 * The QFindDialogController class manages the interactions between the QFindDialog GUI component 
 * and the file data for search operations. It provides functionalities to find, find next, 
 * and find previous occurrences of byte sequences in the file. It also handles user input events 
 * and updates the view accordingly.
 */
class QFindDialogController : public QDialog
{
    Q_OBJECT
public:
	/**
     * @brief Constructs a QFindDialogController object.
     * @param pFindDialog Pointer to the QFindDialog object that this controller will manage.
     */
    QFindDialogController(QFindDialog* pFindDialog);

	/**
     * @brief Destroys the QFindDialogController object.
     */
    virtual ~QFindDialogController();

	/**
     * @brief Retrieves the byte array from the current selection or input.
     * @return The byte array representing the selected or input data.
     */
    QByteArray getStringData();

signals:
	/**
     * @brief Signal emitted to find all occurrences of a regex pattern in the byte array.
     * @param byteArray The byte array to search.
     * @param plstPosition List to store positions of occurrences.
     * @param iStartOffset The start offset for the search.
     * @param iEndOffset The end offset for the search.
     */
	void findAllOccurrencesRegex(QByteArray byteArray, QList<qint64>* plstPosition, qint64 iStartOffset, qint64 iEndOffset);

	/**
     * @brief Signal emitted to select data at a specified offset and size.
     * @param offset The offset to start the selection.
     * @param size The size of the selection.
     */
	void selectData(qint64 offset, qint64 size);

public slots:
	/**
     * @brief Updates the view.
     */
	void updateView();

	/**
     * @brief Moves the view to a specified row.
     * @param iRow The row to move to.
     */
	void moveToRow(int iRow);

	/**
     * @brief Initiates the find operation.
     */
    void find();

	/**
     * @brief Finds the next occurrence.
     */
	void findNext();

	/**
     * @brief Finds the previous occurrence.
     */
	void findPrevious();

	/**
     * @brief Handles selection changes in the hexadecimal editor.
     * @param pHexEditor Pointer to the hexadecimal editor.
     * @param pHumanEditor Pointer to the human-readable editor.
     */
	void handleSelectionChangedHex(QPlainTextEdit* pHexEditor, QPlainTextEdit* pHumanEditor);

	/**
     * @brief Handles selection changes in the human-readable editor.
     * @param pHumanEditor Pointer to the human-readable editor.
     * @param pHexEditor Pointer to the hexadecimal editor.
     */
	void handleSelectionChangedHuman(QPlainTextEdit* pHumanEditor, QPlainTextEdit* pHexEditor);

	/**
     * @brief Removes the hexadecimal editor.
     * @param pHexEditor Pointer to the hexadecimal editor.
     */
	void removeHexEditor(QPlainTextEdit* pHexEditor);

	/**
     * @brief Removes the human-readable editor.
     * @param pHumanEditor Pointer to the human-readable editor.
     */
	void removeHumanEditor(QPlainTextEdit* pHumanEditor);
	
	/**
     * @brief Inserts a character into the human-readable editor.
     * @param pHumanEditor Pointer to the human-readable editor.
     * @param keyText The character to insert.
     */
	void insertCharHumanEditor(QPlainTextEdit* pHumanEditor, QString& keyText);

	/**
     * @brief Inserts a character into the hexadecimal editor.
     * @param pHexEditor Pointer to the hexadecimal editor.
     * @param keyText The character to insert.
     */
	void insertCharHexEditor(QPlainTextEdit* pHexEditor, QString& keyText);

private:
    QFindDialog* m_pFindDialog; /**< Pointer to the QFindDialog object. */

    QByteArray m_byteArray; /**< The byte array for the current selection or input. */

	qint64 m_iDataSize; /**< Size of the data. */
	qint64 m_iDataPos; /**< Position of the data. */

	bool m_bChanged; /**< Indicates if the data has been changed. */

	int m_iBytePerLine; /**< Number of bytes per line in the view. */
	int m_iTotalRowCount; /**< Total number of rows in the view. */
	int m_iVisibleRowCount; /**< Number of visible rows in the view. */

	QList<qint64> m_lstPositions; /**< List of positions of occurrences. */
	int m_iPositionsSize; /**< Size of the list of positions. */
	qint64 m_iListIndex; /**< Current index in the list of positions. */

	/**
     * @brief Handles key press events in the human-readable editor.
     * @param keyEvent Pointer to the key event.
     * @return True if the event was handled, false otherwise.
     */
	bool keyPressHumanEditor(QKeyEvent* keyEvent);

	/**
     * @brief Handles key press events in the hexadecimal editor.
     * @param keyEvent Pointer to the key event.
     * @return True if the event was handled, false otherwise.
     */
	bool keyPressHexEditor(QKeyEvent* keyEvent);

	/**
     * @brief Handles events in the hexadecimal editor.
     * @param obj Pointer to the object receiving the event.
     * @param event Pointer to the event.
     * @return True if the event was handled, false otherwise.
     */
	bool eventHexEditor(QObject *obj, QEvent *event); 

	/**
     * @brief Handles events in the human-readable editor.
     * @param obj Pointer to the object receiving the event.
     * @param event Pointer to the event.
     * @return True if the event was handled, false otherwise.
     */
	bool eventHumanEditor(QObject *obj, QEvent *event); 

	/**
     * @brief Updates the display data.
     */
	void updateDisplayData();

	/**
     * @brief Updates the text starting from a specified offset.
     * @param iStartOffset The start offset for updating the text.
     */
	void updateText(qint64 iStartOffset);
};

#endif /* QFINDDIALOGCONTROLLER_H_ */
