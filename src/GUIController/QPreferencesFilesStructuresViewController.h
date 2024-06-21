#ifndef SRC_GUICONTROLLER_QPREFERENCESFILESSTRUCTURESVIEWCONTROLLER_H_
#define SRC_GUICONTROLLER_QPREFERENCESFILESSTRUCTURESVIEWCONTROLLER_H_

#include <QObject>
#include <QListWidgetItem>
#include <QDir>
#include <QColor>

class QPreferencesFilesStructuresView;

class QPreferencesFilesStructuresViewController : public QObject
{
	Q_OBJECT
public:
	QPreferencesFilesStructuresViewController(QPreferencesFilesStructuresView* pPreferencesFilesStructuresView);
	virtual ~QPreferencesFilesStructuresViewController();

	int getRow();

public slots:
	void handleItemClicked(QListWidgetItem *pItem);
	void handleAddFile();
	void handleRemoveFile();

signals:
	void updateFile();
	void changedPreferencesStatusBar(QString szText);
	
private:
    QPreferencesFilesStructuresView* m_pPreferencesFilesStructuresView; 
	int m_iRow;
	QDir m_dataDir;

	void loadStructureFileList(const QString& szDirPath);
	void reloadStructureFileList();
	void copyFile(const QString& szSourcePath, const QString& szDestPath);
};

#endif /* SRC_GUICONTROLLER_QPREFERENCESFILESSTRUCTURESVIEWCONTROLLER_H_ */
