/*
 * QOpenFileEvent.cpp
 *
 *  Created on: 1 août 2018
 *      Author: ebeuque
 */

#include "QOpenFileEvent.h"

QOpenFileEvent::QOpenFileEvent(const QString& szFilePath)
	: QUserEvent(QUserEvent::OpenFileEvent)
{
	m_szFilePath = szFilePath;
}

QOpenFileEvent::~QOpenFileEvent()
{

}

