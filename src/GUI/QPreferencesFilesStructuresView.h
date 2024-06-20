#ifndef SRC_GUI_QPREFERENCESFILESSTRUCTURESVIEW_H_
#define SRC_GUI_QPREFERENCESFILESSTRUCTURESVIEW_H_

#include <QWidget>
#include <QListWidget>
#include <QPushButton>

class QPreferencesFilesStructuresView : public QWidget
{
	Q_OBJECT
public:
	QPreferencesFilesStructuresView(QWidget* pParent = NULL);
	virtual ~QPreferencesFilesStructuresView();

    QListWidget* getTableWidget() const;
	QPushButton* getRemoveButton() const;
	QPushButton* getAddButton() const;

private:
    QListWidget* m_pFileTable;
	QPushButton* m_pAddButton;
	QPushButton* m_pRemoveButton;
};

#endif /* SRC_GUI_QPREFERENCESFILESSTRUCTURESVIEW_H_ */
