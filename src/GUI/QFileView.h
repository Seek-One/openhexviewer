/*
 * QFileView.h
 *
 *  Created on: 18 juil. 2018
 *      Author: ebeuque
 */

#ifndef SRC_GUI_QFILEVIEW_H_
#define SRC_GUI_QFILEVIEW_H_

#include <QWidget>

class QTextEdit;
class QPlainTextEdit;
class QScrollBar;

/**
 * @brief A widget for displaying and editing file contents in different formats.
 * 
 * The QFileView class provides functionalities for viewing and editing file 
 * contents in hexadecimal and human-readable formats. It includes methods 
 * for setting and getting various properties and handling text selections.
 */
class QFileView : public QWidget
{
	Q_OBJECT
public:
	/**
     * @brief Constructs a new QFileView object.
     * 
     * @param pParent The parent widget of the QFileView. Defaults to nullptr.
     */
	QFileView(QWidget* pParent = NULL);

	/**
     * @brief Destroys the QFileView object.
     */
	virtual ~QFileView();

	/**
     * @brief Sets the maximum number of rows.
     * 
     * @param iMax The maximum number of rows.
     */
	void setRowCount(int iMax);

	/**
     * @brief Sets the current row.
     * 
     * @param iRow The current row number.
     */
	void setCurrentRow(int iRow);

	/**
     * @brief Gets the number of bytes per line.
     * 
     * @return The number of bytes per line.
     */
	int getBytePerLine() const;

	/**
     * @brief Gets the number of visible rows.
     * 
     * @return The number of visible rows.
     */
	int getVisibleRowCount() const;

	/**
     * @brief Sets the offset text.
     * 
     * @param szText The offset text to set.
     */
	void setOffsetText(const QString& szText);

	/**
     * @brief Sets the hexadecimal text.
     * 
     * @param szText The hexadecimal text to set.
     */
	void setHexText(const QString& szText);

	/**
     * @brief Sets the human-readable text.
     * 
     * @param szText The human-readable text to set.
     */
	void setHumanText(const QString& szText);

	/**
     * @brief Moves to the specified row.
     * 
     * @param iRow The row number to move to.
     */
	void moveToRow(int iRow);
	
	/**
     * @brief Selects text within a specific range.
     * 
     * @param iPosStart The start position of the selection.
     * @param iPosEnd The end position of the selection.
     * @param iStartNbRow The starting number of the row.
     * @param iNbRow The number of rows to select.
     */
	void selectText(int iPosStart, int iPosEnd, int iStartNbRow, int iNbRow);

	/**
     * @brief Selects text across multiple rows.
     * 
     * @param iPosStart The start position of the selection.
     * @param iPosEnd The end position of the selection.
     * @param iRowStart The starting row number.
     * @param iRowEnd The ending row number.
     * @param iNbBytePerLine The number of bytes per line.
     */
	void selectTextRow(int iPosStart, int iPosEnd, int iRowStart, int iRowEnd, int iNbBytePerLine);

	/**
     * @brief Gets the hexadecimal text editor.
     * 
     * @return A pointer to the hexadecimal text editor.
     */
	QPlainTextEdit* getHexEditor();

	/**
     * @brief Gets the human-readable text editor.
     * 
     * @return A pointer to the human-readable text editor.
     */
	QPlainTextEdit* getHumanEditor();

signals:
	/**
     * @brief Signal emitted when the size of the widget changes.
     */
	void sizeChanged();

	/**
     * @brief Signal emitted when the current row changes.
     * 
     * @param iRow The new row number.
     */
	void rowChanged(int iRow);

	/**
     * @brief Signal emitted when the hexadecimal text changes.
     * 
     * @param pHexEditor A pointer to the hexadecimal text editor.
     */
	void textChangedHex(QPlainTextEdit* pHexEditor);
	
	/**
     * @brief Signal emitted when the human-readable text changes.
     * 
     * @param pHumanEditor A pointer to the human-readable text editor.
     */
	void textChangedHuman(QPlainTextEdit* pHumanEditor);

	/**
     * @brief Signal emitted when the selection changes in the hexadecimal editor.
     * 
     * @param pHexEditor A pointer to the hexadecimal text editor.
     * @param pHumanEditor A pointer to the human-readable text editor.
     */
	void selectionChangedHex(QPlainTextEdit* pHexEditor, QPlainTextEdit* pHumanEditor);

	/**
     * @brief Signal emitted when the selection changes in the human-readable editor.
     * 
     * @param pHumanEditor A pointer to the human-readable text editor.
     * @param pHexEditor A pointer to the hexadecimal text editor.
     */
	void selectionChangedHuman(QPlainTextEdit* pHumanEditor, QPlainTextEdit* pHexEditor);

	/**
     * @brief Signal emitted when the cursor position changes in the hexadecimal editor.
     * 
     * @param pHexEditor A pointer to the hexadecimal text editor.
     * @param pHumanEditor A pointer to the human-readable text editor.
     */
	void cursorChangedHex(QPlainTextEdit* pHexEditor, QPlainTextEdit* pHumanEditor);

	/**
     * @brief Signal emitted when the cursor position changes in the human-readable editor.
     * 
     * @param pHumanEditor A pointer to the human-readable text editor.
     * @param pHexEditor A pointer to the hexadecimal text editor.
     */
	void cursorChangedHuman(QPlainTextEdit* pHumanEditor, QPlainTextEdit* pHexEditor);

	// void addNewByteHex(QPlainTextEdit* m_pHexEditor);
	// void removeByteHex(QPlainTextEdit* m_pHexEditor);
	// void addNewByteHuman(QPlainTextEdit* m_pHumanEditor, QString szText);
	// void removeByteHuman(QPlainTextEdit* m_pHumanEditor);

protected:
	/**
     * @brief Handles resize events for the widget.
     * 
     * @param event The resize event.
     */
	void resizeEvent(QResizeEvent *event) override;

	/**
     * @brief Event filter for handling specific events.
     * 
     * @param obj The object receiving the event.
     * @param event The event to be handled.
     * @return True if the event was handled, false otherwise.
     */
	bool eventFilter(QObject *obj, QEvent *event) override;

private:
	QPlainTextEdit* m_pOffsetEditor; /**< The editor for offset text. */
	QPlainTextEdit* m_pHexEditor; /**< The editor for hexadecimal text. */
	QPlainTextEdit* m_pHumanEditor; /**< The editor for human-readable text. */
	QScrollBar* m_pScrollBar; /**< The scroll bar for navigating through the text. */
	
	/**
     * @brief Handles events specific to the hexadecimal editor.
     * 
     * @param obj The object receiving the event.
     * @param event The event to be handled.
     * @return True if the event was handled, false otherwise.
     */
	bool eventHexEditor(QObject *obj, QEvent *event); 

	/**
     * @brief Handles events specific to the human-readable editor.
     * 
     * @param obj The object receiving the event.
     * @param event The event to be handled.
     * @return True if the event was handled, false otherwise.
     */
	bool eventHumanEditor(QObject *obj, QEvent *event); 
};

#endif /* SRC_GUI_QFILEVIEW_H_ */
