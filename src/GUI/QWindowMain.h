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
#include <QGraphicsEffect>

class QAction;
class QFileView;
class QFileStructureView;
class QBytesView;
class QDropAreaWidget;

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
     * @brief Gets the drop area widget.
     * 
     * @return A pointer to the QDropAreaWidget object.
     */
	QDropAreaWidget* getDropArea() const;

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

	/**
     * @brief Checks if a file is currently open.
     * 
     * @return True if a file is open, false otherwise.
     */
	bool isFileOpen() const;

	/**
     * @brief Sets the file open status.
     * 
     * @param bOpen True if a file is open, false otherwise.
     */
	void setFileOpen(bool bOpen);

	/**
     * @brief Enables or disables the blur effect.
     * 
     * @param bEnable True to enable the blur effect, false to disable it.
     */
	void enableBlurEffect(bool bEnable);

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

	/**
     * @brief Signal emitted when files are dropped onto the window.
     * @param filePath The path of the dropped file.
     */
	void fileDropped(const QString& filePath);

	/**
     * @brief Signal emitted when drag enter event occurs.
     * @param event The drag enter event.
     */
	void dragEnterTriggered(QDragEnterEvent* event);

	/**
     * @brief Signal emitted when drag leave event occurs.
     * @param event The drag leave event.
     */
	void dragLeaveTriggered(QDragLeaveEvent* event);

	/**
     * @brief Signal emitted when drop event occurs.
     * @param event The drop event.
     */
	void dropTriggered(QDropEvent* event);

protected:
	/**
     * @brief Handles the close event for the main window.
     * 
     * @param event The close event.
     */
	void closeEvent(QCloseEvent* event);

	/**
     * @brief Handles drag enter events.
     * 
     * @param event The drag enter event.
     */
	void dragEnterEvent(QDragEnterEvent* event);

	/**
     * @brief Handles drag leave events.
     * 
     * @param event The drag leave event.
     */
	void dragLeaveEvent(QDragLeaveEvent* event);

	/**
     * @brief Handles drag move events.
     * 
     * @param event The drag move event.
     */
	void dragMoveEvent(QDragMoveEvent* event);

	/**
     * @brief Handles drop events.
     * 
     * @param event The drop event.
     */
	void dropEvent(QDropEvent* event);

	/**
     * @brief Handles resize events.
     * 
     * @param event The resize event.
     */
	void resizeEvent(QResizeEvent* event);

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
	QDropAreaWidget* m_pDropArea; /**< Drop area widget. */
	bool m_bFileOpen; /**< Indicates if a file is currently open. */
};

#endif /* SRC_MAINWINDOW_H_ */
