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
#include <QMessageBox>
#include <QInputDialog>

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
        for (QString szFile : listSelectedFiles) {
            QFileInfo tFileInfo(szFile);
            copyFile(szFile, m_dataDir.path() + "/" + tFileInfo.fileName());
        }
	}
    reloadStructureFileList();
    emit updateFile();
}

void QPreferencesFilesStructuresViewController::handleRemoveFile()
{
    QString fileName = m_pPreferencesFilesStructuresView->getTableWidget()->item(m_iRow)->text();
    
    bool bConfigPath = QFile::exists(m_dataDir.path() + "/" + fileName);
    bool bDataPath = QFile::exists("./data/structure_files/" + fileName);
    
    if (bConfigPath || bDataPath) {
        if (QFile::remove(m_dataDir.path() + "/" + fileName) || QFile::remove("./data/structure_files/" + fileName)) {
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
    QString szDestTemp = szDestPath;
    QFile sourceFile(szSourcePath);

    if (!sourceFile.exists()) {
        qWarning() << "[Preferences] Source file does not exist";
        emit changedPreferencesStatusBar(tr("Source file does not exist"));
        return;
    }

    if (!sourceFile.permissions().testFlag(QFileDevice::ReadUser)) {
        qWarning() << "[Preferences] No read permission for source file";
        emit changedPreferencesStatusBar(tr("No read permission for source file"));
        return;
    }

    QDir destDir = QFileInfo(szDestTemp).absoluteDir();
    if (!destDir.exists()) {
        qDebug() << "[Preferences] Destination directory does not exist. Creating...";
        emit changedPreferencesStatusBar(tr("Destination directory does not exist. Creating..."));

        if (!destDir.mkpath(".")) {
            qWarning() << "[Preferences] Failed to create destination directory";
            emit changedPreferencesStatusBar(tr("Failed to create destination directory"));
            return;
        }
    }

    while (QFile::exists(szDestTemp)) {
        qWarning() << "[Preferences] File with the same name already exists in the destination folder";
        emit changedPreferencesStatusBar(tr("File with the same name already exists in the destination folder"));
        QMessageBox msgBox;
        msgBox.setText(tr("The file already exists."));
        msgBox.setInformativeText(tr("Do you want to change the name or remove the older one?"));
        QPushButton *changeNameButton = msgBox.addButton(tr("Change Name"), QMessageBox::ActionRole);
        msgBox.addButton(tr("Remove Older"), QMessageBox::ActionRole);
        msgBox.exec();

        if (msgBox.clickedButton() == changeNameButton) {
            bool ok;
            QString newFileName = QInputDialog::getText(nullptr, tr("Change File Name"),
                                                        tr("New File Name:"), QLineEdit::Normal,
                                                        QFileInfo(szDestTemp).fileName(), &ok);
            if (ok && !newFileName.isEmpty()) {
                szDestTemp = QFileInfo(destDir, newFileName).absoluteFilePath();
            } else {
                qWarning() << "[Preferences] New file name was not provided.";
                emit changedPreferencesStatusBar(tr("New file name was not provided"));
                return;
            }
        } else {
            if (!QFile::remove(szDestTemp)) {
                qWarning() << "[Preferences] Failed to remove existing destination file";
                emit changedPreferencesStatusBar(tr("Failed to remove existing destination file"));
                return;
            }
        }
    }

    if (!QFile::copy(szSourcePath, szDestTemp)) {
        qWarning() << "[Preferences] Failed to copy file";
        emit changedPreferencesStatusBar(tr("Failed to copy file"));
        return;
    }

    qDebug() << "[Preferences] File copied successfully";
    emit changedPreferencesStatusBar(tr("File copied successfully"));
    return;
}