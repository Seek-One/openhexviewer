#ifndef QFINDDIALOG_H_
#define QFINDDIALOG_H_

#include <QDialog>
#include <QString>
#include <QLabel>
#include <QScrollBar>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QPlainTextEdit>

class QAbstractButton;
class QPlainTextEdit;

class QFindDialog : public QDialog
{
    Q_OBJECT
public:
    QFindDialog(QWidget * parent = 0);
    virtual ~QFindDialog();
    
    void setRowCount(int iMax);
    
	int getBytePerLine() const;
	int getVisibleRowCount() const;

	void setHexText(const QString& szText);
	void setHumanText(const QString& szText);

	void moveToRow(int iRow);
    
    void setLabelNbOcc(qint64 iIndexOcc, qint64 iNbOcc);

signals:
	void sizeChanged();
	void rowChanged(int iRow);

    void findNext();
    void findPrevious();  
    void accepted();

    void textChangedHex(QPlainTextEdit* pHexEditor);
	void textChangedHuman(QPlainTextEdit* pHumanEditor);
	void selectionChangedHex(QPlainTextEdit* pHexEditor, QPlainTextEdit* pHumanEditor);
	void selectionChangedHuman(QPlainTextEdit* pHumanEditor, QPlainTextEdit* pHexEditor);
	void cursorChangedHex(QPlainTextEdit* pHexEditor, QPlainTextEdit* pHumanEditor);
	void cursorChangedHuman(QPlainTextEdit* pHumanEditor, QPlainTextEdit* pHexEditor);

    void removeHumanEditor(QPlainTextEdit* pHumanEditor);
    void removeHexEditor(QPlainTextEdit* pHexEditor);
protected:
	void resizeEvent(QResizeEvent *event) override;

	bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QPlainTextEdit* m_pHexEditor;
    QPlainTextEdit* m_pHumanEditor;
    QScrollBar* m_pScrollBar;

    QDialogButtonBox* m_pButtonBox;
    QPushButton* m_pButtonFindNext;
    QLabel* m_pLabelNbOcc;
    QPushButton* m_pButtonFindPrevious;

	bool eventHexEditor(QObject *obj, QEvent *event); 
	bool eventHumanEditor(QObject *obj, QEvent *event); 
};

#endif /* QFINDDIALOG_H_ */
