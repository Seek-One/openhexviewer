/*
 * QFileViewController.h
 *
 *  Created on: 18 juil. 2018
 *      Author: ebeuque
 */

#ifndef SRC_GUICONTROLLER_QFILEVIEWCONTROLLER_H_
#define SRC_GUICONTROLLER_QFILEVIEWCONTROLLER_H_

#include <QString>
#include <QFile>
#include <QTextCharFormat>
#include "GUIModel/QEditorModificationList.h"

class QFileView;
class QPlainTextEdit;
class QKeyEvent;

/**
 * @brief The controller class for the file view.
 * 
 * The QFileViewController class manages interactions between the QFileView GUI component 
 * and the file data. It provides functionalities for opening, closing, and saving files,
 * as well as selecting and manipulating file data. It also handles user input events 
 * and updates the view accordingly.
 */
class QFileViewController : public QObject
{
	Q_OBJECT
public:
	/**
     * @brief Constructs a QFileViewController object.
     * @param pFileView Pointer to the QFileView object that this controller will manage.
     */
	QFileViewController(QFileView* pFileView);

	/**
     * @brief Destroys the QFileViewController object.
     */
	virtual ~QFileViewController();

	/**
     * @brief Opens a file.
     * @param szFilePath The path of the file to open.
     * @return True if the file was opened successfully, false otherwise.
     */
	bool openFile(const QString& szFilePath);

	/**
     * @brief Saves the currently opened file.
     * @return True if the file was saved successfully, false otherwise.
     */
	bool saveFile();

	/**
     * @brief Closes the currently opened file.
     */
	void closeFile();

	/**
     * @brief Selects file data from a specified offset and size.
     * @param offset The offset to start the selection.
     * @param size The size of the selection.
     */
	void selectFileData(qint64 offset, qint64 size);

	/**
     * @brief Retrieves the current selection offset and size.
     * @param offset The offset of the selection.
     * @param size The size of the selection.
     */
	void getSelectionOffset(qint64& offset, qint64& size);

signals:
	/**
     * @brief Signal emitted when the bytes selection changes.
     * @param offset The new selection offset.
     * @param size The new selection size.
     */
	void onBytesSelectionChanged(qint64 offset, qint64 size); 

	/**
     * @brief Signal emitted when the bytes change.
     * @param szHex The new bytes in hexadecimal format.
     */
	void onBytesChanged(QString szHex);

	/**
     * @brief Signal emitted when a file is opened.
     */
	void fileOpened();

	/**
     * @brief Signal emitted when a file is closed.
     */
	void fileClosed();

	/**
     * @brief Signal emitted to indicate that changes should be applied.
     */
 	void doChanges();

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
     * @brief Toggles text coloring.
     * @param bIsChecked True to enable coloring, false to disable.
     */
	void colorText(bool bIsChecked);

	/**
     * @brief Handles text selection.
     * @param szText The selected text.
     */
	void selection(QString& szText);
	
	/**
     * @brief Undoes the last operation.
     */
	void undo();

	/**
     * @brief Handles text changes in the hexadecimal editor.
     * @param pHexEditor Pointer to the hexadecimal editor.
     */
	void handleTextChangedHex(QPlainTextEdit* pHexEditor);

	/**
     * @brief Handles text changes in the human-readable editor.
     * @param pHumanEditor Pointer to the human-readable editor.
     */
	void handleTextChangedHuman(QPlainTextEdit* pHumanEditor);

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
     * @brief Handles cursor changes in the hexadecimal editor.
     * @param pHexEditor Pointer to the hexadecimal editor.
     * @param pHumanEditor Pointer to the human-readable editor.
     */
	void handleCursorChangedHex(QPlainTextEdit* pHexEditor, QPlainTextEdit* pHumanEditor); 

	/**
     * @brief Handles cursor changes in the human-readable editor.
     * @param pHumanEditor Pointer to the human-readable editor.
     * @param pHexEditor Pointer to the hexadecimal editor.
     */
	void handleCursorChangedHuman(QPlainTextEdit* pHumanEditor, QPlainTextEdit* pHexEditor); 
	
	// void addNewByteHex(QPlainTextEdit* pHexEditor);
	// void removeByteHex(QPlainTextEdit* m_pHexEditor);
	// void addNewByteHuman(QPlainTextEdit* m_pHumanEditor, QString szText);
	// void removeByteHuman(QPlainTextEdit* m_pHumanEditor);

	/**
     * @brief Finds all occurrences of a regex pattern in the byte array.
     * @param byteArray The byte array to search.
     * @param plstPositions List to store positions of occurrences.
     * @param iStartOffset The start offset for the search.
     * @param iEndOffset The end offset for the search.
     */
	void findAllOccurrencesRegex(const QByteArray &byteArray, QList<qint64>* plstPositions, qint64 iStartOffset, qint64 iEndOffset);
private:
	QFileView* m_pFileView; /**< Pointer to the QFileView object. */

	// Current file
	bool m_bIsFileOpen; /**< Indicates if a file is currently open. */
	QFile m_file; /**< The currently opened file. */
	qint64 m_iFileSize; /**< The size of the currently opened file. */
	qint64 m_iFilePos; /**< The current position in the file. */

	// Current display
	int m_iBytePerLine; /**< Number of bytes per line in the view. */
	int m_iTotalRowCount; /**< Total number of rows in the view. */
	int m_iVisibleRowCount; /**< Number of visible rows in the view. */

	bool m_bHighLight; /**< Indicates if highlighting is enabled. */

	QEditorModificationList* m_pModifications; /**< List of modifications to the file. */

	/**
     * @brief Updates the display data.
     */
	void updateDisplayData();

	/**
     * @brief Reads data from the file starting at a specified offset.
     * @param iStartOffset The start offset for reading.
     * @return True if the data was read successfully, false otherwise.
     */
	bool readFile(qint64 iStartOffset);

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

    QTextCharFormat m_nonAsciiFormat; /**< Format for non-ASCII characters. */
    QTextCharFormat m_otherAsciiFormat; /**< Format for other ASCII characters. */
    QTextCharFormat m_whitespaceFormat; /**< Format for whitespace characters. */
    QTextCharFormat m_printableFormat; /**< Format for printable characters. */
	QTextCharFormat m_nullFormat; /**< Format for null characters. */
    QTextCharFormat m_offsetFormat; /**< Format for offset characters. */
};

#endif /* SRC_GUICONTROLLER_QFILEVIEWCONTROLLER_H_ */
