#ifndef QFINDDIALOGCONTROLLER_H_
#define QFINDDIALOGCONTROLLER_H_

#include <QDialog>
#include <QString>

// #include "QFileViewController.h"

class QFindDialog;
class QAbstractButton;
class QPlainTextEdit; // Add this line for forward declaration

class QFindDialogController : public QDialog
{
    Q_OBJECT
public:
    QFindDialogController(QFindDialog* pFindDialog);
    virtual ~QFindDialogController();

public slots:
    void find();
	void findNext();
	void findPrevious();

private:
    QFindDialog* m_pFindDialog;

	// QFileViewController m_pFileViewController;

	// qint64 m_iFindSize;
	// qint64 m_iArrayIndex;
	// qint64* m_piArray;
	// qint64 m_iArraySize;
};

#endif /* QFINDDIALOGCONTROLLER_H_ */
