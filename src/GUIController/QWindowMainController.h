/*
 * QWindowMainController.h
 *
 *  Created on: 18 july 2018
 *      Author: ebeuque
 */

#ifndef SRC_GUICONTROLLER_QWINDOWMAINCONTROLLER_H_
#define SRC_GUICONTROLLER_QWINDOWMAINCONTROLLER_H_

#include <QObject>
#include <QCloseEvent> 

#include "GUI/QWindowPreferences.h"


class QWindowMain;
class QFileViewController;
class QFileStructureViewController;
class QBytesViewController;
class QGoToBytesController;
class QFindDialogController;
class QFileExportViewController;
class QWindowPreferencesController;

class QWindowMainController : public QObject
{
	Q_OBJECT
public:
	QWindowMainController();
	virtual ~QWindowMainController();

	void init(QWindowMain* pMainWindow);

signals:
	void colorText(bool bIsChecked);

public slots:
	void openFile();
	void saveFile();
	void about();
	void find();
	void preferences();
	void goToBytes();
	void color();
	void exportSelection();

	void selectFileData(qint64 offset, qint64 size);

	void onBytesSelectionChanged(qint64 start, qint64 end);

	void close(QCloseEvent* event);
	
	void doChanges();

public:
	void openFile(const QString& szFilePath);
	

private:
	QWindowMain* m_pMainWindow;

	bool m_bSavedChanges;

	QFileViewController* m_pFileViewController;
	QWindowPreferencesController* m_pWindowPreferencesController;
	QFileStructureViewController* m_pFileStructureViewController;
	QBytesViewController* m_pBytesViewController;
	QGoToBytesController* m_pGoToBytesController;
	QFindDialogController* m_pFindDialogController;
	QFileExportViewController* m_pFileExportViewController;
	QWindowPreferences* m_windowPreferences;

	void actionUsable(bool bEnabled);
};

#endif /* SRC_GUICONTROLLER_QWINDOWMAINCONTROLLER_H_ */
