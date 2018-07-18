/*
 * MainWindow.h
 *
 *  Created on: 7 juin 2016
 *      Author: echopin
 */

#ifndef SRC_MAINWINDOW_H_
#define SRC_MAINWINDOW_H_

#include <QMainWindow>
#include <QStackedWidget>

class QWindowMain : public QMainWindow
{
	Q_OBJECT
public:
	QWindowMain(QWidget* parent = 0);
	virtual ~QWindowMain();
};

#endif /* SRC_MAINWINDOW_H_ */
