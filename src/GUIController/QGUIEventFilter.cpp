/*
 * QGUIEventFilter.cpp
 *
 *  Created on: 1 août 2018
 *      Author: ebeuque
 */

#include "Event/QUserEvent.h"
#include "Event/QOpenFileEvent.h"

#include "GUIController/QWindowMainController.h"

#include "QGUIEventFilter.h"

QGUIEventFilter::QGUIEventFilter()
{
	m_pWindowMainController = NULL;
}

QGUIEventFilter::~QGUIEventFilter()
{

}

bool QGUIEventFilter::eventFilter(QObject *object, QEvent *event)
{
	if(event->type() == QUserEvent::OpenFileEvent){
		QOpenFileEvent* pOpenFileEvent = (QOpenFileEvent*)event;
		m_pWindowMainController->openFile(pOpenFileEvent->m_szFilePath);
		return true;
	}

	return false;
}

void QGUIEventFilter::setWindowMainController(QWindowMainController* pWindowMainController)
{
	m_pWindowMainController = pWindowMainController;
}
