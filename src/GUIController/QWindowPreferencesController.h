#ifndef SRC_GUICONTROLLER_QWINDOWPREFERENCESCONTROLLER_H_
#define SRC_GUICONTROLLER_QWINDOWPREFERENCESCONTROLLER_H_

#include <QObject>
#include "QPreferencesFilesStructuresViewController.h"

class QWindowPreferences;

class QWindowPreferencesController : public QObject
{
	Q_OBJECT
public:
	QWindowPreferencesController(QWindowPreferences* pPreferencesWindow);
	virtual ~QWindowPreferencesController();

private:
	QWindowPreferences* m_pPreferencesWindow;
    QPreferencesFilesStructuresViewController* m_pPreferencesFilesStructuresViewController;
};

#endif /* SRC_GUICONTROLLER_QWINDOWPREFERENCESCONTROLLER_H_ */
