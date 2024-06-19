/*
 * MainWindow.h
 *
 *  Created on: 18 july 2018
 *      Author: ebeuque
 */

#ifndef SRC_MAINWINDOW_H_
#define SRC_MAINWINDOW_H_

#include <QMainWindow>
#include <QStackedWidget>

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

	QAction* getOpenAction() const;
	QAction* getQuitAction() const;
	QAction* getAboutAction() const;
	QAction* getGoToAction() const;
	QAction* getFindAction() const;

	QFileView* getFileView() const;
	QFileStructureView* getFileStructureView() const;
	QBytesView* getBytesView() const;

	void setStatusBarText(const QString& szText);

private:
	void createMenu(); // Creates a menu bar

	// List of actions
	QAction* m_pOpenAction;
	QAction* m_pQuitAction;
	QAction* m_pGoToAction;
	QAction* m_pFindAction;
	QAction* m_pAboutAction;

	QFileView* m_pFileView;
	QFileStructureView* m_pFileStructureView;
	QBytesView* m_pBytesView;
	QStatusBar* m_pStatusBar;
};

#endif /* SRC_MAINWINDOW_H_ */
