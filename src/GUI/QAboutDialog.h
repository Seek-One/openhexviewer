/*
 * QAboutDialog.h
 *
 *  Created on: 18 july 2018
 *      Author: ebeuque
 */

#ifndef QABOUTDIALOG_H_
#define QABOUTDIALOG_H_

#include <QDialog>
#include <QString>

class QAbstractButton;

class QAboutDialog : public QDialog
{
	Q_OBJECT
public:
	QAboutDialog(QWidget * parent = 0);
	virtual ~QAboutDialog();

public slots:
	void menuAboutQtClicked();

private:
	QAbstractButton* addLibraryButton(QLayout* pLayout, QWidget* pParent, const QString& szIconFile, const QString& szName);
};

#endif /* QABOUTDIALOG_H_ */
