/*
 * QWindowMainController.h
 *
 *  Created on: 18 july 2018
 *      Author: ebeuque
 */

#ifndef SRC_GUICONTROLLER_QWINDOWMAINCONTROLLER_H_
#define SRC_GUICONTROLLER_QWINDOWMAINCONTROLLER_H_

#include <QObject>

class QWindowMain;
class QFileViewController;
class QFileStructureViewController;
class QBytesViewController;
class QGoToBytesController;
class QFindDialogController;

class QWindowMainController : public QObject
{
	Q_OBJECT
public:
	QWindowMainController();
	virtual ~QWindowMainController();

	void init(QWindowMain* pMainWindow);

public slots:
	void openFile();
	void saveFile();
	void about();
	void find();
	void goToBytes();

	void selectFileData(qint64 offset, qint64 size);

	void onBytesSelectionChanged(qint64 start, qint64 end);

public:
	void openFile(const QString& szFilePath);

private:
	QWindowMain* m_pMainWindow;

	QFileViewController* m_pFileViewController;
	QFileStructureViewController* m_pFileStructureViewController;
	QBytesViewController* m_pBytesViewController;
	QGoToBytesController* m_pGoToBytesController;
	QFindDialogController* m_pFindDialogController;
};

#endif /* SRC_GUICONTROLLER_QWINDOWMAINCONTROLLER_H_ */
