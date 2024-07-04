#ifndef SRC_GUI_QPREFERENCESFILESSTRUCTURESVIEW_H_
#define SRC_GUI_QPREFERENCESFILESSTRUCTURESVIEW_H_

#include <QWidget>
#include <QListWidgetItem>
#include <QPushButton>

class QPreferencesFilesStructuresView : public QWidget
{
	Q_OBJECT
public:
	QPreferencesFilesStructuresView(QWidget* pParent = NULL);
	virtual ~QPreferencesFilesStructuresView();

    QListWidget* getTableWidget() const;

signals:
	void listWidgetItemClicked(QListWidgetItem* pItem);
	void addButtonClicked();
	void removeButtonClicked();
	
private:
    QListWidget* m_pFileTable;
};

#endif /* SRC_GUI_QPREFERENCESFILESSTRUCTURESVIEW_H_ */
