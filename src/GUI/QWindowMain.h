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

class QWindowMain : public QMainWindow
{
	Q_OBJECT
public:
	QWindowMain(QWidget* parent = 0);
	virtual ~QWindowMain();

	QFileView* getFileView() const;
	QFileStructureView* getFileStructureView() const;
	QBytesView* getBytesView() const;
	bool getColorIsChecked();

	void setStatusBarText(const QString& szText);
	void actionFileUsable(bool bEnabled);
signals:
    void mainWindowClosed(QCloseEvent* event);

	void openFileClicked();
	void preferencesClicked();
	void saveFileClicked();
	void quitClicked();
	void aboutClicked();
	void undoClicked();
	void goToClicked();
	void findClicked();
	void exportSelectionClicked();
	void colorClicked();

protected:
	void closeEvent(QCloseEvent* event);

private:
	void createMenu(); // Creates a menu bar

	// List of actions
	QAction* m_pSaveAction;
	QAction* m_pGoToAction;
	QAction* m_pFindAction;
	QAction* m_pColorAction;
	QAction* m_pExportSelectionAction;

	QFileView* m_pFileView;
	QFileStructureView* m_pFileStructureView;
	QBytesView* m_pBytesView;
	QStatusBar* m_pStatusBar;
};

#endif /* SRC_MAINWINDOW_H_ */
