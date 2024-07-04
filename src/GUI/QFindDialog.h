#ifndef QFINDDIALOG_H_
#define QFINDDIALOG_H_


#include <QObject>

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
#define USE_NO_QREGEXP
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#else
#include <QRegExp>
#include <QRegExpValidator>
#endif

#include <QDialog>
#include <QString>
#include <QLabel>
#include <QScrollBar>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QBoxLayout>
#include <QComboBox>

typedef enum {
    NORMAL,
    CUSTOM
} ValueType;

typedef struct {
    ValueType type;
    int base;
} Value;

typedef struct {
    Value startOffset;
    Value endOffset;
} OffsetValue;

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

    OffsetValue getOffsets();

    QString getStartOffset();
    QString getEndOffset();
    QComboBox* getComboPosOcc();

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

    void insertCharHumanEditor(QPlainTextEdit* pHumanEditor, QString& keyText);
    void insertCharHexEditor(QPlainTextEdit* pHexEditor, QString& keyText);

    void offsetChanged();
    void comboOffsetChanged(int iIndex);

protected:
	void resizeEvent(QResizeEvent *event) override;

	bool eventFilter(QObject *obj, QEvent *event) override;

private:

    OffsetValue m_offsets;

    QLineEdit* m_pStartEdit;
    QLineEdit* m_pEndEdit;

    QPlainTextEdit* m_pHexEditor;
    QPlainTextEdit* m_pHumanEditor;
    QScrollBar* m_pScrollBar;

    QDialogButtonBox* m_pButtonBox;
    QPushButton* m_pButtonFindNext;
    QComboBox* m_pComboPosOcc;
    QLabel* m_pLabelNbOcc;
    QPushButton* m_pButtonFindPrevious;

	bool eventHexEditor(QObject *obj, QEvent *event); 
	bool eventHumanEditor(QObject *obj, QEvent *event); 

    void createSettings(QBoxLayout* pParentLayout);
    void createButton(QBoxLayout* pParentLayout);
    void createView(QBoxLayout* pParentLayout);
};

#endif /* QFINDDIALOG_H_ */
