#ifndef __QFILEEXPORTVIEWCONTROLLER_H__
#define __QFILEEXPORTVIEWCONTROLLER_H__

#include <QDialog>
#include <QString>

class QFileExportView;
class QAbstractButton;

class QFileExportViewController : public QDialog
{
	Q_OBJECT
public:
	QFileExportViewController(QFileExportView* pFileExportView);
	virtual ~QFileExportViewController();

    void init(qint64 iStart, qint64 iSize);


signals:
    void changeOffset(qint64 iOffset, qint64 size);
    void selection(QString& szText);

public slots:
    void saveSelection();
    void changeBase();

    void startOffsetChanged(const QString &szText);
    void sizeOffsetChanged(const QString &szText);
    void endOffsetChanged(const QString &szText);

private:
    QFileExportView* m_pExportView;

    qint64 m_iStart;
    qint64 m_iSize;
    qint64 m_iEnd;
};
#endif /* __QFILEEXPORTVIEWCONTROLLER_H_ */