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
#include <QtEndian>
#include <QTableWidget>
#include <QAbstractItemView>

#include "Global/QtCompat.h"

#include "GUI/QPreferencesFilesStructuresView.h"

#include "QPreferencesFilesStructuresViewController.h"

QPreferencesFilesStructuresViewController::QPreferencesFilesStructuresViewController(QPreferencesFilesStructuresView* pPreferencesFilesStructuresView)
{
	m_pPreferencesFilesStructuresView = pPreferencesFilesStructuresView;

    QTableWidget* pTable = m_pPreferencesFilesStructuresView->getTableWidget();

    pTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    pTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    pTable->setColumnCount(1);

    QDir dir = QDir::home();
#ifdef UNIX
	dir.setPath(dir.filePath(".config"));
	dir.setPath(dir.filePath(APPLICATION_PACKAGE_NAME));
	dir.setPath(dir.filePath("structure_files"));
#else
	dir.setPath(dir.filePath(APPLICATION_PACKAGE_NAME));
	dir.setPath(dir.filePath("structure_files"));
#endif

    QStringList listFilters;
    listFilters << "*.xml";
    QStringList entryList = dir.entryList(listFilters, QDir::Files, QDir::Name);

    pTable->setRowCount(entryList.size());

	QStringList::const_iterator iter;
    int i = 0;
    for (iter = entryList.constBegin(); iter != entryList.constEnd(); ++iter) {
        QTableWidgetItem *fileNameItem = new QTableWidgetItem(dir.filePath(*iter));
        pTable->setItem(i++, 0, fileNameItem);
    }

    pTable->resizeColumnsToContents();
}

QPreferencesFilesStructuresViewController::~QPreferencesFilesStructuresViewController()
{

}


// QString szFilePath = "./data/structure_files";
// 	loadStructureFileList(szFilePath);

// 	// Load from user config dir
// 	QDir dir = QDir::home();
// #ifdef UNIX
// 	dir.setPath(dir.filePath(".config"));
// 	dir.setPath(dir.filePath(APPLICATION_PACKAGE_NAME));
// 	dir.setPath(dir.filePath("structure_files"));
// #else
// 	dir.setPath(dir.filePath(APPLICATION_PACKAGE_NAME));
// 	dir.setPath(dir.filePath("structure_files"));
// #endif
// 	loadStructureFileList(dir.path());
// }

// void QFileStructureViewController::loadStructureFileList(const QString& szDirPath)
// {
// 	qDebug("[GUI] Loading structure files from: %s", qPrintable(szDirPath));

// 	QDir dir(szDirPath);

// 	QStringList listFilters;
// 	listFilters << "*.xml";
// 	QStringList entryList = dir.entryList(listFilters, QDir::Files, QDir::Name);

// 	QComboBox* pComboBox = m_pFileStructureView->getStructureFileComboBox();

// 	QStringList::const_iterator iter;
// 	for(iter = entryList.constBegin(); iter != entryList.constEnd(); ++iter)
// 	{
// 		QString szFilePath = dir.filePath(*iter);
// 		pComboBox->addItem(*iter, szFilePath);
// 	}