/*
 * QUserEvent.h
 *
 *  Created on: 3 oct. 2012
 *      Author: ebeuque
 */

#ifndef QUSEREVENT_H_
#define QUSEREVENT_H_

#include <QEvent>

/*
 *
 */
class QUserEvent : public QEvent {
public:

	static const QEvent::Type OpenFileEvent = static_cast<QEvent::Type>(QEvent::User + 1);

	QUserEvent(int type);
	virtual ~QUserEvent() = 0;
};

#endif /* QUSEREVENT_H_ */
