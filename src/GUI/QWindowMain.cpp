/*
 * MainWindow.cpp
 *
 *  Created on: 7 juin 2016
 *      Author: echopin
 */

#include <QVBoxLayout>
#include <QMenu>
#include <QMenuBar>

#include "QWindowMain.h"

QWindowMain::QWindowMain(QWidget* parent)
	: QMainWindow(parent)
{
    //Set minimum window size
    setMinimumSize(800, 600);
}

QWindowMain::~QWindowMain()
{

}
