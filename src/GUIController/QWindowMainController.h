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

class QWindowMainController : public QObject
{
	Q_OBJECT
public:
	QWindowMainController();
	virtual ~QWindowMainController();

	void init(QWindowMain* pMainWindow);

public slots:
	void openFile();
	void about();

	void selectFileData(qint64 offset, qint64 size);

public:
	void openFile(const QString& szFilePath);

private:
	QWindowMain* m_pMainWindow;

	QFileViewController* m_pFileViewController;
	QFileStructureViewController* m_pFileStructureViewController;
};

#endif /* SRC_GUICONTROLLER_QWINDOWMAINCONTROLLER_H_ */
