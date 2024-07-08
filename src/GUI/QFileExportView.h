#ifndef __QFILEEXPORTVIEW_H__
#define __QFILEEXPORTVIEW_H__

#include <QObject>

#include <QDialog>
#include <QString>
#include <QRadioButton>
#include <QLineEdit>

/**
 * @brief A dialog for exporting selection with specific offset settings
 * 
 * The QFileExportView class provides a dialog interface for setting
 * start, size and end offsets for selection export operations. It includes
 * radio buttons to select the base (hex or decimal) for the offsets
 */
class QFileExportView : public QDialog
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new QFileExportView object
     * 
     * @param parent The parent widget of the QFileExportView. Defaults to nullptr
     */
    QFileExportView(QWidget * parent = 0);

    /**
     * @brief Destroy the QFileExportView object 
     */
    virtual ~QFileExportView();
    
    /**
     * @brief Get the current base used for offfsets(e.g., hex or decimal)
     * 
     * @return int The base value (e.g., 16 for hex, 10 for decimal)
     */
    int getBase();

    /**
     * @brief Sets the offset values for start, size and end 
     * 
     * @param szStart The start offset
     * @param szSize The size offset
     * @param szEnd The end offset
     */
    void setOffset(QString szStart, QString szSize, QString szEnd);

signals:
    /**
     * @brief Signal emitted when the save selection action is triggered
     */
    void saveSelection();
    /**
     * @brief Signal emitted when the offset values are changed
     */
    void changeOffset();
    /**
     * @brief Signal emitted when the base value is changed
     */
    void changeBase();

    /**
     * @brief Signal emitted when the start offset is changed
     * 
     * @param const QString& The new start offset value  
     */
    void startOffsetChanged(const QString&);
    /**
     * @brief Signal emitted when the end offset is changed
     *
     * @param const QString& The new end offset value 
     */
    void endOffsetChanged(const QString&);
    /**
     * @brief Signal emitted when the size offset is changed
     * 
     * @param const QString& The new size offset value
     */
    void sizeOffsetChanged(const QString&);

private:
    QLineEdit* m_pStartOffset; /**< Line edit for the start offset */
    QLineEdit* m_pSizeOffset; /**< Line edit for the size offset */
    QLineEdit* m_pEndOffset; /**< Line edit fot the end offset */

    int m_iRadioButton = 16; /**< Default base value for offsets (16 for hex) */

    /**
     * @brief Slot to handle radio button clicks
     * 
     */
    void onRadioButtonClicked();
};

#endif // __QFILEEXPORTVIEW_H__