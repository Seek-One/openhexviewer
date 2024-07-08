#ifndef QFINDDIALOG_H_
#define QFINDDIALOG_H_


#include <QObject>

#include <QDialog>
#include <QString>
#include <QLabel>
#include <QScrollBar>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QBoxLayout>
#include <QComboBox>

/**
 * @brief Enumeration for value types.
 */
typedef enum {
    NORMAL, /**< Normal value type. */
    CUSTOM  /**< Custom value type. */
} ValueType;

/**
 * @brief Structure for value representation.
 */
typedef struct {
    ValueType type; /**< Type of the value. */
    int base;       /**< Base of the value (e.g., 16 for hexadecimal). */
} Value;

/**
 * @brief Structure for offset values.
 */
typedef struct {
    Value startOffset; /**< Start offset value. */
    Value endOffset;   /**< End offset value. */
} OffsetValue;

class QAbstractButton;
class QPlainTextEdit;

/**
 * @brief A dialog for finding text within a file.
 * 
 * The QFindDialog class provides a dialog interface for finding text
 * in hexadecimal and human-readable formats. It includes methods for
 * setting text, moving to specific rows, and handling offset values.
 */
class QFindDialog : public QDialog
{
    Q_OBJECT
public:
    /**
     * @brief Constructs a new QFindDialog object.
     * 
     * @param parent The parent widget of the QFindDialog. Defaults to nullptr.
     */
    QFindDialog(QWidget * parent = 0);

    /**
     * @brief Destroys the QFindDialog object.
     */
    virtual ~QFindDialog();
    
    /**
     * @brief Sets the maximum number of rows.
     * 
     * @param iMax The maximum number of rows.
     */
    void setRowCount(int iMax);
    
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
     * @brief Sets the label for the number of occurrences.
     * 
     * @param iIndexOcc The current index of occurrences.
     * @param iNbOcc The total number of occurrences.
     */
    void setLabelNbOcc(qint64 iIndexOcc, qint64 iNbOcc);

    /**
     * @brief Gets the offset values.
     * 
     * @return The offset values.
     */
    OffsetValue getOffsets();

    /**
     * @brief Gets the start offset value.
     * 
     * @return The start offset value as a QString.
     */
    QString getStartOffset();

    /**
     * @brief Gets the end offset value.
     * 
     * @return The end offset value as a QString.
     */
    QString getEndOffset();

    /**
     * @brief Gets the combo box for position occurrences.
     * 
     * @return A pointer to the QComboBox for position occurrences.
     */
    QComboBox* getComboPosOcc();

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
     * @brief Signal emitted to find the next occurrence.
     */
    void findNext();

    /**
     * @brief Signal emitted to find the previous occurrence.
     */
    void findPrevious();  

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

    /**
     * @brief Signal emitted to remove the human-readable editor.
     * 
     * @param pHumanEditor A pointer to the human-readable text editor.
     */
    void removeHumanEditor(QPlainTextEdit* pHumanEditor);

    /**
     * @brief Signal emitted to remove the hexadecimal editor.
     * 
     * @param pHexEditor A pointer to the hexadecimal text editor.
     */
    void removeHexEditor(QPlainTextEdit* pHexEditor);

    /**
     * @brief Signal emitted to insert a character into the human-readable editor.
     * 
     * @param pHumanEditor A pointer to the human-readable text editor.
     * @param keyText The character to insert.
     */
    void insertCharHumanEditor(QPlainTextEdit* pHumanEditor, QString& keyText);

    /**
     * @brief Signal emitted to insert a character into the hexadecimal editor.
     * 
     * @param pHexEditor A pointer to the hexadecimal text editor.
     * @param keyText The character to insert.
     */
    void insertCharHexEditor(QPlainTextEdit* pHexEditor, QString& keyText);

    /**
     * @brief Signal emitted when the offset values change.
     */
    void offsetChanged();

    /**
     * @brief Signal emitted when the combo box selection changes.
     * 
     * @param iIndex The new index of the combo box.
     */
    void comboOffsetChanged(int iIndex);

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

    OffsetValue m_offsets; /**< The offset values. */

    QLineEdit* m_pStartEdit; /**< Line edit for the start offset. */
    QLineEdit* m_pEndEdit; /**< Line edit for the end offset. */

    QPlainTextEdit* m_pHexEditor; /**< The editor for hexadecimal text. */
    QPlainTextEdit* m_pHumanEditor; /**< The editor for human-readable text. */
    QScrollBar* m_pScrollBar; /**< The scroll bar for navigating through the text. */

    QDialogButtonBox* m_pButtonBox; /**< The button box containing dialog buttons. */
    QPushButton* m_pButtonFindNext; /**< The button for finding the next occurrence. */
    QComboBox* m_pComboPosOcc; /**< The combo box for position occurrences. */
    QLabel* m_pLabelNbOcc; /**< The label for displaying the number of occurrences. */
    QPushButton* m_pButtonFindPrevious; /**< The button for finding the previous occurrence. */

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

    /**
     * @brief Creates the settings layout.
     * 
     * @param pParentLayout The parent layout to which the settings will be added.
     */
    void createSettings(QBoxLayout* pParentLayout);

    /**
     * @brief Creates the button layout.
     * 
     * @param pParentLayout The parent layout to which the buttons will be added.
     */
    void createButton(QBoxLayout* pParentLayout);

    /**
     * @brief Creates the view layout.
     * 
     * @param pParentLayout The parent layout to which the view will be added.
     */
    void createView(QBoxLayout* pParentLayout);
};

#endif /* QFINDDIALOG_H_ */
