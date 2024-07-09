#ifndef SRC_GUICONTROLLER_QWINDOWPREFERENCESCONTROLLER_H_
#define SRC_GUICONTROLLER_QWINDOWPREFERENCESCONTROLLER_H_

#include <QObject>
#include "QPreferencesFilesStructuresViewController.h"

class QWindowPreferences;

/**
 * @brief The controller class for the preferences window.
 * 
 * The QWindowPreferencesController class manages the interactions between the QWindowPreferences
 * GUI component and the application logic for handling user preferences. It provides access to 
 * specific preferences controllers and ensures the synchronization between the GUI and underlying
 * data.
 */
class QWindowPreferencesController : public QObject
{
	Q_OBJECT
public:
	/**
     * @brief Constructs a QWindowPreferencesController object.
     * @param pPreferencesWindow Pointer to the QWindowPreferences object that this controller will manage.
     */
	QWindowPreferencesController(QWindowPreferences* pPreferencesWindow);

	/**
     * @brief Destroys the QWindowPreferencesController object.
     */
	virtual ~QWindowPreferencesController();

	/**
     * @brief Retrieves the preferences files structures view controller.
     * @return Pointer to the QPreferencesFilesStructuresViewController object.
     */
	QPreferencesFilesStructuresViewController* getPreferencesFilesStructuresViewController();

private:
	QWindowPreferences* m_pPreferencesWindow; /**< Pointer to the preferences window. */
    QPreferencesFilesStructuresViewController* m_pPreferencesFilesStructuresViewController; /**< Pointer to the preferences files structures view controller. */
};

#endif /* SRC_GUICONTROLLER_QWINDOWPREFERENCESCONTROLLER_H_ */
