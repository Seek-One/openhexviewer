/*
 * MainWindow.h
 *
 *  Created on: 18 july 2018
 *      Author: ebeuque
 */

#ifndef SRC_MAINWINDOW_H_
#define SRC_MAINWINDOW_H_

#include <QMainWindow>
#include <QCloseEvent>

class QAction;
class QFileView;
class QFileStructureView;
class QBytesView;

/**
 * @brief The main window class for the application.
 * 
 * The QWindowMain class provides the main window interface for the application,
 * including file views, file structure views, byte views, and menu actions.
 */
class QWindowMain : public QMainWindow
{
	Q_OBJECT
public:
	/**
     * @brief Constructs a new QWindowMain object.
     * 
     * @param parent The parent widget of the QWindowMain. Defaults to nullptr.
     */
	QWindowMain(QWidget* parent = 0);

	/**
     * @brief Destroys the QWindowMain object.
     */
	virtual ~QWindowMain();

	/**
     * @brief Gets the file view.
     * 
     * @return A pointer to the QFileView object.
     */
	QFileView* getFileView() const;

	/**
     * @brief Gets the file structure view.
     * 
     * @return A pointer to the QFileStructureView object.
     */
	QFileStructureView* getFileStructureView() const;

	/**
     * @brief Gets the bytes view.
     * 
     * @return A pointer to the QBytesView object.
     */
	QBytesView* getBytesView() const;

	/**
     * @brief Checks if the color option is enabled.
     * 
     * @return True if the color option is enabled, false otherwise.
     */
	bool getColorIsChecked();

	/**
     * @brief Sets the text of the status bar.
     * 
     * @param szText The text to be displayed in the status bar.
     */
	void setStatusBarText(const QString& szText);

	/**
     * @brief Enables or disables file-related actions.
     * 
     * @param bEnabled True to enable the actions, false to disable them.
     */
	void actionFileUsable(bool bEnabled);
signals:
	/**
     * @brief Signal emitted when the main window is closed.
     * 
     * @param event The close event.
     */
    void mainWindowClosed(QCloseEvent* event);

	/**
     * @brief Signal emitted when the open file action is triggered.
     */
    void openFileClicked();

    /**
     * @brief Signal emitted when the preferences action is triggered.
     */
    void preferencesClicked();

    /**
     * @brief Signal emitted when the save file action is triggered.
     */
    void saveFileClicked();

    /**
     * @brief Signal emitted when the quit action is triggered.
     */
    void quitClicked();

    /**
     * @brief Signal emitted when the about action is triggered.
     */
    void aboutClicked();

    /**
     * @brief Signal emitted when the undo action is triggered.
     */
    void undoClicked();

    /**
     * @brief Signal emitted when the go to action is triggered.
     */
    void goToClicked();

    /**
     * @brief Signal emitted when the find action is triggered.
     */
    void findClicked();

    /**
     * @brief Signal emitted when the export selection action is triggered.
     */
    void exportSelectionClicked();

    /**
     * @brief Signal emitted when the color option is toggled.
     */
    void colorClicked();

protected:
	/**
     * @brief Handles the close event for the main window.
     * 
     * @param event The close event.
     */
	void closeEvent(QCloseEvent* event);

private:
	/**
     * @brief Creates the menu bar and its actions.
     */
	void createMenu();

	// List of actions
	QAction* m_pSaveAction; /**< Action for saving a file. */
	QAction* m_pGoToAction; /**< Action for going to a specific byte. */
	QAction* m_pFindAction; /**< Action for finding content in the file. */
	QAction* m_pColorAction; /**< Action for toggling color options. */
	QAction* m_pExportSelectionAction; /**< Action for exporting selected content. */

	QFileView* m_pFileView; /**< File view widget. */
	QFileStructureView* m_pFileStructureView; /**< File structure view widget. */
	QBytesView* m_pBytesView; /**< Bytes view widget. */
	QStatusBar* m_pStatusBar; /**< Status bar widget. */
};

#endif /* SRC_MAINWINDOW_H_ */
