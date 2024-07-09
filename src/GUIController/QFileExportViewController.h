#ifndef __QFILEEXPORTVIEWCONTROLLER_H__
#define __QFILEEXPORTVIEWCONTROLLER_H__

#include <QDialog>
#include <QString>

class QFileExportView;
class QAbstractButton;

/**
 * @brief The controller class for the file export view.
 * 
 * The QFileExportViewController class manages the interactions between the
 * QFileExportView GUI component and the underlying data model. It handles
 * user inputs for exporting file data, including setting offsets and sizes
 * for the data to be exported.
 */
class QFileExportViewController : public QDialog
{
	Q_OBJECT
public:
    /**
     * @brief Constructs a QFileExportViewController object.
     * @param pFileExportView Pointer to the QFileExportView object that this controller will manage.
     */
    QFileExportViewController(QFileExportView* pFileExportView);

    /**
     * @brief Destroys the QFileExportViewController object.
     */
    virtual ~QFileExportViewController();

    /**
     * @brief Initializes the export parameters.
     * @param iStart The start offset for the export.
     * @param iSize The size of the data to be exported.
     */
    void init(qint64 iStart, qint64 iSize);

signals:
    /**
     * @brief Signal emitted when the offset or size changes.
     * @param iOffset The new offset value.
     * @param size The new size value.
     */
    void changeOffset(qint64 iOffset, qint64 size);

    /**
     * @brief Signal emitted with the selected text.
     * @param szText The selected text.
     */
    void selection(QString& szText);

public slots:
    /**
     * @brief Saves the current selection.
     */
    void saveSelection();

    /**
     * @brief Changes the base offset.
     */
    void changeBase();

    /**
     * @brief Slot to handle changes in the start offset.
     * @param szText The new start offset as a string.
     */
    void startOffsetChanged(const QString &szText);

    /**
     * @brief Slot to handle changes in the size offset.
     * @param szText The new size offset as a string.
     */
    void sizeOffsetChanged(const QString &szText);

    /**
     * @brief Slot to handle changes in the end offset.
     * @param szText The new end offset as a string.
     */
    void endOffsetChanged(const QString &szText);

private:
    QFileExportView* m_pExportView;

    qint64 m_iStart; /**< The start offset for the export. */
    qint64 m_iSize; /**< The size of the data to be exported. */
    qint64 m_iEnd; /**< The end offset for the export. */
};
#endif /* __QFILEEXPORTVIEWCONTROLLER_H_ */