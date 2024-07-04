#ifndef __QFILEEXPORTVIEW_H__
#define __QFILEEXPORTVIEW_H__

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
#include <QRadioButton>
#include <QLineEdit>

class QAbstractButton;

class QFileExportView : public QDialog
{
    Q_OBJECT
public:
    QFileExportView(QWidget * parent = 0);
    virtual ~QFileExportView();
    
    int getBase();

    void setOffset(QString szStart, QString szSize, QString szEnd);

signals:
    void saveSelection();
    void changeOffset();
    void changeBase();

    void startOffsetChanged(const QString&);
    void endOffsetChanged(const QString&);
    void sizeOffsetChanged(const QString&);

private:
    QLineEdit* m_pStartOffset; 
    QLineEdit* m_pSizeOffset; 
    QLineEdit* m_pEndOffset; 

    int m_iRadioButton = 16;

    void onRadioButtonClicked();
};

#endif // __QFILEEXPORTVIEW_H__