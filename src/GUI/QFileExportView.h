#ifndef __QFILEEXPORTVIEW_H__
#define __QFILEEXPORTVIEW_H__

#include <QDialog>
#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QLineEdit>
#include <QDialogButtonBox>

class QAbstractButton;

class QFileExportView : public QDialog
{
    Q_OBJECT
public:
    QFileExportView(QWidget * parent = 0);
    virtual ~QFileExportView();
    
    int getBase();

    void setOffset(QString szStart, QString szSize, QString szEnd);

    QLineEdit* getStartOffset();
    QLineEdit* getSizeOffset();
    QLineEdit* getEndOffset();

signals:
    void saveSelection();
    void changeOffset();
    void changeBase();

private:
    QLineEdit* m_pStartOffset; 
    QLineEdit* m_pSizeOffset; 
    QLineEdit* m_pEndOffset; 

    int m_iRadioButton = 16;

    void onRadioButtonClicked();
};

#endif // __QFILEEXPORTVIEW_H__