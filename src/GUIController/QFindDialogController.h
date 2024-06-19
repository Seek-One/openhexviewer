#ifndef QFINDDIALOGCONTROLLER_H_
#define QFINDDIALOGCONTROLLER_H_

#include <QDialog>
#include <QString>

#include "../GUI/QFindDialog.h"
#include "QFileViewController.h"

class QFindDialog;
class QPlainTextEdit;

class QFindDialogController : public QDialog
{
    Q_OBJECT
public:
    QFindDialogController(QFindDialog* pFindDialog);
    virtual ~QFindDialogController();

signals:
	void findAllOccurrencesRegex(QString szData, QList<qint64>* plstPosition);
	void selectData(qint64 offset, qint64 size);

public slots:
    void find();
	void findNext();
	void findPrevious();

private:
    QFindDialog* m_pFindDialog;

	QFileViewController* m_pFileViewController;

	QList<qint64> m_lstPositions;
	int m_iPositionsSize;
	qint64 m_iFindSize;
	qint64 m_iListIndex;
};

#endif /* QFINDDIALOGCONTROLLER_H_ */
