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

/**
 * @brief The event filter class for GUI events.
 * 
 * The QGUIEventFilter class intercepts and processes events for the application's main window. 
 * It acts as an intermediary to handle specific types of events before they reach their target 
 * objects, allowing custom event handling logic to be implemented.
 */
class QGUIEventFilter : public QObject
{
public:
	/**
     * @brief Constructs a QGUIEventFilter object.
     */
	QGUIEventFilter();

	/**
     * @brief Destroys the QGUIEventFilter object.
     */
	virtual ~QGUIEventFilter();

	/**
     * @brief Filters events for the watched objects.
     * 
     * This method is called for every event sent to an object that this filter is installed on.
     * @param object Pointer to the object receiving the event.
     * @param event Pointer to the event.
     * @return True if the event is handled and should not be further processed, false otherwise.
     */
	bool eventFilter(QObject *object, QEvent *event);

	/**
     * @brief Sets the main window controller for this event filter.
     * @param pWindowMainController Pointer to the QWindowMainController object.
     */
	void setWindowMainController(QWindowMainController* pWindowMainController);

private:
	QWindowMainController* m_pWindowMainController; /**< Pointer to the main window controller. */
};

#endif /* SRC_GUICONTROLLER_QGUIEVENTFILTER_H_ */
