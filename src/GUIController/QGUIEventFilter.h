/*
 * QGUIEventFilter.h
 *
 *  Created on: 1 août 2018
 *      Author: ebeuque
 */

#ifndef SRC_GUICONTROLLER_QGUIEVENTFILTER_H_
#define SRC_GUICONTROLLER_QGUIEVENTFILTER_H_

#include <QObject>

class QWindowMainController;

class QGUIEventFilter : public QObject
{
public:
	QGUIEventFilter();
	virtual ~QGUIEventFilter();

	bool eventFilter(QObject *object, QEvent *event);

	void setWindowMainController(QWindowMainController* pWindowMainController);

private:
	QWindowMainController* m_pWindowMainController;
};

#endif /* SRC_GUICONTROLLER_QGUIEVENTFILTER_H_ */
