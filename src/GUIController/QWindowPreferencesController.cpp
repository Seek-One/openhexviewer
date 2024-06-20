#include <QCoreApplication>
#include <QAction>
#include <QFileDialog>

#include "Global/QtCompat.h"

#include "GUI/QWindowPreferences.h"
#include "GUI/QPreferencesFilesStructuresView.h"

#include "QPreferencesFilesStructuresViewController.h"
#include "QWindowPreferencesController.h"

QWindowPreferencesController::QWindowPreferencesController(QWindowPreferences* pPreferencesWindow)
{
	m_pPreferencesWindow = pPreferencesWindow;
    m_pPreferencesFilesStructuresViewController = new QPreferencesFilesStructuresViewController(pPreferencesWindow->getFileStructureView()); 
}

QWindowPreferencesController::~QWindowPreferencesController()
{
	if(m_pPreferencesWindow){
		delete m_pPreferencesWindow;
		m_pPreferencesWindow = NULL;
	}
}
