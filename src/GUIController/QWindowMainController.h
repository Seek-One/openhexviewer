/*
 * QWindowMainController.h
 *
 *  Created on: 18 july 2018
 *      Author: ebeuque
 */

#ifndef SRC_GUICONTROLLER_QWINDOWMAINCONTROLLER_H_
#define SRC_GUICONTROLLER_QWINDOWMAINCONTROLLER_H_

#include <QObject>
#include <QCloseEvent> 

#include "GUI/QWindowPreferences.h"


class QWindowMain;
class QFileViewController;
class QFileStructureViewController;
class QBytesViewController;
class QGoToBytesController;
class QFindDialogController;
class QFileExportViewController;
class QWindowPreferencesController;

/**
 * @brief The main controller class for the application's main window.
 * 
 * The QWindowMainController class manages the interactions between the QWindowMain GUI component and 
 * various sub-controllers, handling user actions and coordinating updates to the view.
 */
class QWindowMainController : public QObject
{
	Q_OBJECT
public:
	/**
     * @brief Constructs a QWindowMainController object.
     */
	QWindowMainController();

	/**
     * @brief Destroys the QWindowMainController object.
     */
	virtual ~QWindowMainController();

	/**
     * @brief Initializes the main window controller with the main window object.
     * @param pMainWindow Pointer to the QWindowMain object.
     */
	void init(QWindowMain* pMainWindow);

signals:
	/**
     * @brief Signal emitted to change the text color.
     * @param bIsChecked Whether the text color should be changed.
     */
	void colorText(bool bIsChecked);

public slots:
	/**
     * @brief Opens a file using a file dialog.
     */
	void openFile();

	/**
     * @brief Saves the currently open file.
     */
	void saveFile();

	/**
     * @brief Shows the "About" dialog.
     */
	void about();

	/**
     * @brief Opens the find dialog.
     */
	void find();

	/**
     * @brief Opens the preferences window.
     */
	void preferences();

	/**
     * @brief Opens the "Go To Bytes" dialog.
     */
	void goToBytes();

	/**
     * @brief Toggles text coloring.
     */
	void color();

	/**
     * @brief Opens the export selection dialog.
     */
	void exportSelection();

	/**
     * @brief Selects file data at the specified offset and size.
     * @param offset The starting offset of the selection.
     * @param size The size of the selection.
     */
	void selectFileData(qint64 offset, qint64 size);

	/**
     * @brief Handles the event when the byte selection changes.
     * @param start The starting offset of the selection.
     * @param end The ending offset of the selection.
     */
	void onBytesSelectionChanged(qint64 start, qint64 end);

	/**
     * @brief Handles the close event of the main window.
     * @param event Pointer to the QCloseEvent.
     */
	void close(QCloseEvent* event);
	
	/**
     * @brief Applies changes made to the current file.
     */
	void doChanges();

public:
	/**
     * @brief Opens a file given its path.
     * @param szFilePath The path to the file to be opened.
     */
	void openFile(const QString& szFilePath);
	
private:
	QWindowMain* m_pMainWindow; /**< Pointer to the main window. */

	bool m_bSavedChanges; /**< Indicates whether there are unsaved changes. */

	QFileViewController* m_pFileViewController; /**< Pointer to the file view controller. */
	QWindowPreferencesController* m_pWindowPreferencesController; /**< Pointer to the window preferences controller. */
	QFileStructureViewController* m_pFileStructureViewController; /**< Pointer to the file structure view controller. */
	QBytesViewController* m_pBytesViewController; /**< Pointer to the bytes view controller. */
	QGoToBytesController* m_pGoToBytesController; /**< Pointer to the go-to-bytes controller. */
	QFindDialogController* m_pFindDialogController; /**< Pointer to the find dialog controller. */
	QFileExportViewController* m_pFileExportViewController; /**< Pointer to the file export view controller. */
	QWindowPreferences* m_windowPreferences; /**< Pointer to the window preferences. */

	/**
     * @brief Enables or disables actions in the main window.
     * @param bEnabled Whether the actions should be enabled.
     */
	void actionUsable(bool bEnabled);
};

#endif /* SRC_GUICONTROLLER_QWINDOWMAINCONTROLLER_H_ */
