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

/**
 * @brief Forward declaration of QAbstractButton
 */
class QAbstractButton;

/**
 * @brief A dialog class to display information about the application
 * 
 * The QAboutDialog class provides a standard dialog that shows
 * information about the application, including the libraries udes
 */
class QAboutDialog : public QDialog
{
	Q_OBJECT
public:
	/**
	 * @brief Construct a new QAboutDialog object
	 * 
	 * @param parent The parent widget of the dialog. Defaults to nullprt
	 */
	QAboutDialog(QWidget * parent = 0);
	/**
	 * @brief Destroy the QAboutDialog object
	 */
	virtual ~QAboutDialog();

public slots:
	/**
	 * @brief Slot to handle the 'About Qt' menu click event
	 */
	void menuAboutQtClicked();

private:
	/**
	 * @brief Adds a button representing a library to the dialog
	 * 
	 * @param pLayout The layout to which the button will added
	 * @param pParent The parent widget of the button
	 * @param szIconFile The file path of the button icon
	 * @param szName The name of the library
	 * @return A pointer to the created QAbstractButton
	 */
	QAbstractButton* addLibraryButton(QLayout* pLayout, QWidget* pParent, const QString& szIconFile, const QString& szName);
};

#endif /* QABOUTDIALOG_H_ */
