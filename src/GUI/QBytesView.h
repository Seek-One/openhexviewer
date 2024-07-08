#ifndef SRC_GUI_QBYTESVIEW_H_
#define SRC_GUI_QBYTESVIEW_H_

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QLabel>

/**
 * @brief A widget for viewing and managing byte data in a table format
 * 
 * The QBytesView class provides functionalities to display, modify
 * and manage data in a table view. It includes methods for setting
 * items renaming row headers, resizing columns, and showing warnings
 */
class QBytesView : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Construct a new QBytesView object
     * 
     * @param pParent The parent widget of the QBytesView. Defaults to nullptr
     */
	QBytesView(QWidget* pParent = NULL);

    /**
     * @brief Destroy the QBytesView object
     */
	virtual ~QBytesView() {}
    
    /**
     * @brief Set the text of an item in the specified row
     * 
     * @param iRow The row number
     * @param szText The text to set in the item
     */
    void setItem(int iRow, QString szText);

    /**
     * @brief Rnames the header of the specified row
     * 
     * @param iRow The row number
     * @param szText The new text for the row header
     */
    void renameRowHeaders(int iRow, QString szText);

    /**
     * @brief Resizes all columns in the table to fit their contents
     */
    void resizeColumnsToContents();

    /**
     * @brief Display a warning message
     */
    void showWarning();

    /**
     * @brief Displays the table view
     */
    void showTable();

private:
    QTableView* m_pTableView; /**< The table view widget for displaying data */
    QStandardItemModel* m_pModel; /**< The model for managing the data in the table view */
    QLabel* m_pWarningLabel; /**< The label for displaying warning messages*/

    /**
     * @brief Set the text of an item in the specified row and column
     * 
     * @param iRow The row number
     * @param iColumn The column number
     * @param szText The text to set in the item
     */
    void setItem(int iRow, int iColumn, QString szText);
};

#endif /* SRC_GUI_QBYTESVIEW_H_ */