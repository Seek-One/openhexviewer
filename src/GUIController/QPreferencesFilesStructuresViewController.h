#ifndef SRC_GUICONTROLLER_QPREFERENCESFILESSTRUCTURESVIEWCONTROLLER_H_
#define SRC_GUICONTROLLER_QPREFERENCESFILESSTRUCTURESVIEWCONTROLLER_H_

#include <QObject>
#include <QListWidgetItem>
#include <QDir>
#include <QColor>

class QPreferencesFilesStructuresView;

/**
 * @brief The controller class for managing file structures in the preferences.
 * 
 * The QPreferencesFilesStructuresViewController class manages the interactions between the 
 * QPreferencesFilesStructuresView GUI component and the application logic for handling file 
 * structures in user preferences. It provides functionalities to add, remove, and list files, 
 * and handles user interactions with the file list.
 */
class QPreferencesFilesStructuresViewController : public QObject
{
	Q_OBJECT
public:
	/**
     * @brief Constructs a QPreferencesFilesStructuresViewController object.
     * @param pPreferencesFilesStructuresView Pointer to the QPreferencesFilesStructuresView object that this controller will manage.
     */
	QPreferencesFilesStructuresViewController(QPreferencesFilesStructuresView* pPreferencesFilesStructuresView);

	/**
     * @brief Destroys the QPreferencesFilesStructuresViewController object.
     */
	virtual ~QPreferencesFilesStructuresViewController();

	/**
     * @brief Retrieves the currently selected row in the file list.
     * @return The index of the selected row.
     */
	int getRow();

public slots:
	/**
     * @brief Handles the event when an item in the file list is clicked.
     * @param pItem Pointer to the clicked QListWidgetItem.
     */
	void handleItemClicked(QListWidgetItem *pItem);

	/**
     * @brief Handles the addition of a new file to the list.
     */
	void handleAddFile();

	/**
     * @brief Handles the removal of a file from the list.
     */
	void handleRemoveFile();

signals:
	/**
     * @brief Signal emitted to update the file list.
     */
	void updateFile();

	/**
     * @brief Signal emitted to change the status bar text in preferences.
     * @param szText The new status bar text.
     */
	void changedPreferencesStatusBar(QString szText);
	
private:
    QPreferencesFilesStructuresView* m_pPreferencesFilesStructuresView; /**< Pointer to the preferences files structures view. */
	int m_iRow; /**< The index of the selected row in the file list. */
	QDir m_dataDir; /**< The directory containing the structure files. */

	/**
     * @brief Loads the list of structure files from the specified directory.
     * @param szDirPath The path to the directory containing structure files.
     */
	void loadStructureFileList(const QString& szDirPath);

	/**
     * @brief Reloads the list of structure files.
     */
	void reloadStructureFileList();

	/**
     * @brief Copies a file from the source path to the destination path.
     * @param szSourcePath The source file path.
     * @param szDestPath The destination file path.
     */
	void copyFile(const QString& szSourcePath, const QString& szDestPath);
};

#endif /* SRC_GUICONTROLLER_QPREFERENCESFILESSTRUCTURESVIEWCONTROLLER_H_ */
