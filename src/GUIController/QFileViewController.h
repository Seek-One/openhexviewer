/*
 * QFileViewController.h
 *
 *  Created on: 18 juil. 2018
 *      Author: ebeuque
 */

#ifndef SRC_GUICONTROLLER_QFILEVIEWCONTROLLER_H_
#define SRC_GUICONTROLLER_QFILEVIEWCONTROLLER_H_

#include <QString>
#include <QFile>

class QFileView;

class QFileViewController : public QObject
{
	Q_OBJECT
public:
	QFileViewController(QFileView* pFileView);
	virtual ~QFileViewController();

	bool openFile(const QString& szFilePath);
	void closeFile();

	void selectFileData(qint64 offset, qint64 size);

public slots:
	void updateView();
	void moveToRow(int iRow);

private:
	QFileView* m_pFileView;

	// Current file
	bool m_bIsFileOpen;
	QFile m_file;
	qint64 m_iFileSize;
	qint64 m_iFilePos;

	// Current display
	int m_iBytePerLine;
	int m_iTotalRowCount;
	int m_iVisibleRowCount;

	void updateDisplayData();
	bool readFile(qint64 iStartOffset);
};

#endif /* SRC_GUICONTROLLER_QFILEVIEWCONTROLLER_H_ */
