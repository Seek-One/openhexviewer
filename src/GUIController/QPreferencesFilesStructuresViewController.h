#ifndef SRC_GUICONTROLLER_QPREFERENCESFILESSTRUCTURESVIEWCONTROLLER_H_
#define SRC_GUICONTROLLER_QPREFERENCESFILESSTRUCTURESVIEWCONTROLLER_H_

#include <QObject>

class QPreferencesFilesStructuresView;

class QPreferencesFilesStructuresViewController : public QObject
{
	Q_OBJECT
public:
	QPreferencesFilesStructuresViewController(QPreferencesFilesStructuresView* pPreferencesFilesStructuresView);
	virtual ~QPreferencesFilesStructuresViewController();

private:
    QPreferencesFilesStructuresView* m_pPreferencesFilesStructuresView;    
};

#endif /* SRC_GUICONTROLLER_QPREFERENCESFILESSTRUCTURESVIEWCONTROLLER_H_ */
