/*
 * QOpenFileEvent.h
 *
 *  Created on: 1 août 2018
 *      Author: ebeuque
 */

#ifndef SRC_EVENT_QOPENFILEEVENT_H_
#define SRC_EVENT_QOPENFILEEVENT_H_

#include <QEvent>
#include <QString>

#include "Event/QUserEvent.h"

class QOpenFileEvent : public QUserEvent
{
public:
	QOpenFileEvent(const QString& szFilePath);
	virtual ~QOpenFileEvent();

public:
	QString m_szFilePath;
};

#endif /* SRC_EVENT_QOPENFILEEVENT_H_ */
