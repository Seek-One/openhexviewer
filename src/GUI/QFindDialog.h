#ifndef QFINDDIALOG_H_
#define QFINDDIALOG_H_

#include <QDialog>
#include <QString>
#include <QLabel>
#include <QScrollBar>
#include <QPushButton>
#include <QDialogButtonBox>

#include "QFileView.h"

class QAbstractButton;
class QPlainTextEdit;

class QFindDialog : public QDialog
{
    Q_OBJECT
public:
    QFindDialog(QWidget * parent = 0);
    virtual ~QFindDialog();

    QFileView* getFileView() const;
    
    void setLabelNbOcc(qint64 iIndexOcc, qint64 iNbOcc);

signals:
    void findNext();
    void findPrevious();  
    void accepted();

protected:

private:
    QFileView* m_pFileView;
    QDialogButtonBox* m_pButtonBox;
    QPushButton* m_pButtonFindNext;
    QLabel* m_pLabelNbOcc;
    QPushButton* m_pButtonFindPrevious;
};

#endif /* QFINDDIALOG_H_ */
