/*
 * QUserEvent.cpp
 *
 *  Created on: 3 oct. 2012
 *      Author: ebeuque
 */

#include "QUserEvent.h"

QUserEvent::QUserEvent(int type) : QEvent((QEvent::Type)type)
{

}

QUserEvent::~QUserEvent()
{

}

