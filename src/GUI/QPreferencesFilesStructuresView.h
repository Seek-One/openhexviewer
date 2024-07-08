#ifndef SRC_GUI_QPREFERENCESFILESSTRUCTURESVIEW_H_
#define SRC_GUI_QPREFERENCESFILESSTRUCTURESVIEW_H_

#include <QWidget>
#include <QListWidgetItem>
#include <QPushButton>

/**
 * @brief A view for managing file structures in the preferences dialog.
 * 
 * The QPreferencesFilesStructuresView class provides a widget for displaying
 * and managing file structures within a preferences dialog. It includes methods
 * for accessing the file list widget and emits signals for item clicks and button actions.
 */
class QPreferencesFilesStructuresView : public QWidget
{
	Q_OBJECT
public:
	/**
     * @brief Constructs a new QPreferencesFilesStructuresView object.
     * 
     * @param pParent The parent widget of the QPreferencesFilesStructuresView. Defaults to nullptr.
     */
	QPreferencesFilesStructuresView(QWidget* pParent = NULL);

	/**
     * @brief Destroys the QPreferencesFilesStructuresView object.
     */
	virtual ~QPreferencesFilesStructuresView();

	/**
     * @brief Gets the file list widget.
     * 
     * @return A pointer to the QListWidget containing the file structures.
     */
    QListWidget* getTableWidget() const;

signals:
	/**
     * @brief Signal emitted when an item in the list widget is clicked.
     * 
     * @param pItem A pointer to the clicked QListWidgetItem.
     */
	void listWidgetItemClicked(QListWidgetItem* pItem);
	
	/**
     * @brief Signal emitted when the add button is clicked.
     */
	void addButtonClicked();

	/**
     * @brief Signal emitted when the remove button is clicked.
     */
	void removeButtonClicked();
	
private:
    QListWidget* m_pFileTable; /**< List widget for displaying file structures. */
};

#endif /* SRC_GUI_QPREFERENCESFILESSTRUCTURESVIEW_H_ */
