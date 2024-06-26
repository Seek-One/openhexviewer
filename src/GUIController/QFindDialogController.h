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

    QString getStringData();

signals:
	void findAllOccurrencesRegex(QString szData, QList<qint64>* plstPosition);
	void selectData(qint64 offset, qint64 size);

public slots:
	void updateView();
	void moveToRow(int iRow);

    void find();
	void findNext();
	void findPrevious();

	void handleTextChangedHex(QPlainTextEdit* pHexEditor);
	void handleTextChangedHuman(QPlainTextEdit* pHumanEditor);
	void handleSelectionChangedHex(QPlainTextEdit* pHexEditor, QPlainTextEdit* pHumanEditor);
	void handleSelectionChangedHuman(QPlainTextEdit* pHumanEditor, QPlainTextEdit* pHexEditor);
	void handleCursorChangedHex(QPlainTextEdit* pHexEditor, QPlainTextEdit* pHumanEditor); 
	void handleCursorChangedHuman(QPlainTextEdit* pHumanEditor, QPlainTextEdit* pHexEditor); 
	void removeHexEditor(QPlainTextEdit* pHexEditor);
	void removeHumanEditor(QPlainTextEdit* pHumanEditor);
	
private:
    QFindDialog* m_pFindDialog;

    QString m_szData;

	qint64 m_iDataSize;
	qint64 m_iDataPos;

	int m_iBytePerLine;
	int m_iTotalRowCount;
	int m_iVisibleRowCount;

	QList<qint64> m_lstPositions;
	int m_iPositionsSize;
	qint64 m_iFindSize;
	qint64 m_iListIndex;

	bool keyPressHumanEditor(QKeyEvent* keyEvent);
	bool keyPressHexEditor(QKeyEvent* keyEvent);
	bool eventHexEditor(QObject *obj, QEvent *event); 
	bool eventHumanEditor(QObject *obj, QEvent *event); 

	void updateDisplayData();
	void updateText(qint64 iStartOffset);
};

#endif /* QFINDDIALOGCONTROLLER_H_ */
