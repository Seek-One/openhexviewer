#ifndef SRC_GUI_QPREFERENCESFILESSTRUCTURESVIEW_H_
#define SRC_GUI_QPREFERENCESFILESSTRUCTURESVIEW_H_

#include <QWidget>
#include <QTableWidget>

class QPreferencesFilesStructuresView : public QWidget
{
	Q_OBJECT
public:
	QPreferencesFilesStructuresView(QWidget* pParent = NULL);
	virtual ~QPreferencesFilesStructuresView();

    QTableWidget* getTableWidget() const;

private:

    QTableWidget* m_pFileTable;
};

#endif /* SRC_GUI_QPREFERENCESFILESSTRUCTURESVIEW_H_ */
