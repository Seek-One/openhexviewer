

#ifndef SRC_GUI_QBYTESVIEW_H_
#define SRC_GUI_QBYTESVIEW_H_

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QLabel>

class QBytesView : public QWidget
{
    Q_OBJECT

public:
	QBytesView(QWidget* pParent = NULL);
	virtual ~QBytesView() {}
    
    void setItem(int iRow, QString szText);
    void renameRowHeaders(int iRow, QString szText);
    void resizeColumnsToContents();

    void showWarning();
    void showTable();

private:
    QTableView* m_pTableView;
    QStandardItemModel* m_pModel;
    QLabel* m_pWarningLabel;

    void setItem(int iRow, int iColumn, QString szText);
};

#endif /* SRC_GUI_QBYTESVIEW_H_ */