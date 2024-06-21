#include <QDebug>
#include <QCoreApplication>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_VERSION_H
#include <version.h>
#endif

#include <QFile>
#include <QDir>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QColor>

#include "Global/QtCompat.h"

#include "GUI/QPreferencesFilesStructuresView.h"

#include "QPreferencesFilesStructuresViewController.h"

QPreferencesFilesStructuresViewController::QPreferencesFilesStructuresViewController(QPreferencesFilesStructuresView* pPreferencesFilesStructuresView)
{
	m_pPreferencesFilesStructuresView = pPreferencesFilesStructuresView;
 
    m_dataDir = QDir::home();
    #ifdef UNIX
	m_dataDir.setPath(m_dataDir.filePath(".config"));
	m_dataDir.setPath(m_dataDir.filePath(APPLICATION_PACKAGE_NAME));
	m_dataDir.setPath(m_dataDir.filePath("structure_files"));
#else
	m_dataDir.setPath(m_dataDir.filePath(APPLICATION_PACKAGE_NAME));
	m_dataDir.setPath(m_dataDir.filePath("structure_files"));
#endif

    reloadStructureFileList();
 
    connect(m_pPreferencesFilesStructuresView->getTableWidget(), &QListWidget::itemClicked, this, &QPreferencesFilesStructuresViewController::handleItemClicked);
    connect(m_pPreferencesFilesStructuresView->getAddButton(), &QAbstractButton::clicked, this, &QPreferencesFilesStructuresViewController::handleAddFile);
    connect(m_pPreferencesFilesStructuresView->getRemoveButton(), &QAbstractButton::clicked, this, &QPreferencesFilesStructuresViewController::handleRemoveFile);
}

QPreferencesFilesStructuresViewController::~QPreferencesFilesStructuresViewController()
{
    
}

int QPreferencesFilesStructuresViewController::getRow()
{
    return m_iRow;
}

void QPreferencesFilesStructuresViewController::loadStructureFileList(const QString& szDirPath)
{
    QListWidget* pTable = m_pPreferencesFilesStructuresView->getTableWidget();
    QDir dir(szDirPath);

    QStringList listFilters;
    listFilters << "*.xml";
    QStringList entryList = dir.entryList(listFilters, QDir::Files, QDir::Name);

	QStringList::const_iterator iter;
    for (iter = entryList.constBegin(); iter != entryList.constEnd(); ++iter) {
        QFileInfo tFileInfo(dir.filePath(*iter));
        new QListWidgetItem(tFileInfo.fileName(), pTable);
    }
}

void QPreferencesFilesStructuresViewController::reloadStructureFileList()
{
    m_pPreferencesFilesStructuresView->getTableWidget()->clear();

	QString szFilePath = "./data/structure_files";
    loadStructureFileList(szFilePath);
    loadStructureFileList(m_dataDir.path());
}

void QPreferencesFilesStructuresViewController::handleItemClicked(QListWidgetItem *pItem)
{
    m_iRow = m_pPreferencesFilesStructuresView->getTableWidget()->row(pItem);
}

void QPreferencesFilesStructuresViewController::handleAddFile()
{
    QFileDialog dialog(m_pPreferencesFilesStructuresView);
	dialog.setFileMode(QFileDialog::AnyFile);
	dialog.setViewMode(QFileDialog::Detail);
	dialog.setNameFilter(tr("XML files (*.xml)"));

	if (dialog.exec()){
		QStringList listSelectedFiles;
		listSelectedFiles = dialog.selectedFiles();
        for (QString fileName : listSelectedFiles) {
            copyFile(fileName, m_dataDir.path());
            // copyFile(fileName, "./data/structure_files");
        }
	}
    reloadStructureFileList();
    emit updateFile();
}

void QPreferencesFilesStructuresViewController::handleRemoveFile()
{
    QString fileName = m_pPreferencesFilesStructuresView->getTableWidget()->item(m_iRow)->text();
    
    bool bConfigPath = QFile::exists(m_dataDir.path() + fileName);
    bool bDataPath = QFile::exists("./data/structure_files/" + fileName);
    
    if (bConfigPath || bDataPath) {
        if (QFile::remove(m_dataDir.path() + fileName) || QFile::remove("./data/structure_files/" + fileName)) {
            qDebug() << "[Preferences] File removed successfully";
            emit changedPreferencesStatusBar(tr("File removed successfully"));
        } else {
            qWarning() << "[Preferences] Failed to remove file"; 
            emit changedPreferencesStatusBar(tr("Failed to remove file"));

        }
    } else {
        qWarning() << "[Preferences] File does not exits";
        emit changedPreferencesStatusBar(tr("File does not exists"));
    }
    reloadStructureFileList();
    emit updateFile();
}

void QPreferencesFilesStructuresViewController::copyFile(const QString& szSourcePath, const QString& szDestPath)
{
    qDebug("%s", qPrintable(szDestPath));
    if (!QFile::exists(szSourcePath)) {
        qWarning() << "[Preferences] Source file does not exist";
        emit changedPreferencesStatusBar(tr("Source file does not exist"));
        return;
    }

    if (!QFile::exists(szDestPath)) {
        qWarning() << "[Preferences] Destination file does not exist";
        emit changedPreferencesStatusBar(tr("Destination file does not exist"));
        return;
    }

    if (QFile::copy(szSourcePath, szDestPath)) {
        qDebug() << "[Preferences] File copied successfully";
        emit changedPreferencesStatusBar(tr("File copied successfully"));
    } else {
        qWarning() << "[Preferences] Failed to copy file";
        emit changedPreferencesStatusBar(tr("Failed to copy file"));
    }
}