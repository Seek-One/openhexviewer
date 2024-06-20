#ifndef SRC_PREFERENCESWINDOW_H_
#define SRC_PREFERENCESWINDOW_H_

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>

#include "QPreferencesFilesStructuresView.h"

class QAction;
class QPreferencesFilesStructuresView;

class QWindowPreferences : public QMainWindow
{
	Q_OBJECT
public:
	QWindowPreferences(QWidget* parent = 0);
	virtual ~QWindowPreferences();

    QPreferencesFilesStructuresView* getFileStructureView() const;

private:
    void createStatusBar();
	
    QPreferencesFilesStructuresView* m_pPreferencesFilesStructuresView;
	QPushButton* m_pAddButton;
	QPushButton* m_pRemoveButton;
	QStatusBar* m_pStatusBar;
};

#endif /* SRC_PREFERENCESWINDOW_H_ */
