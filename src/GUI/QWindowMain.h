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

class QWindowMain : public QMainWindow
{
	Q_OBJECT
public:
	QWindowMain(QWidget* parent = 0);
	virtual ~QWindowMain();

	QAction* getOpenAction() const;
	QAction* getQuitAction() const;
	QAction* getAboutAction() const;

private:
	void createMenu(); // Creates a menu bar

	// List of actions
	QAction* m_pOpenAction;
	QAction* m_pQuitAction;
	QAction* m_pAboutAction;
};

#endif /* SRC_MAINWINDOW_H_ */
