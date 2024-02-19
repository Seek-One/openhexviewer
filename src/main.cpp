#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef HAVE_VERSION_H
#include <version.h>
#endif

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QMessageBox>
#include <QLocale>

#include "Global/QtCompat.h"

#include "GUI/QWindowMain.h"
#include "GUIController/QWindowMainController.h"
#include "GUIController/QGUIEventFilter.h"

#include "Event/QOpenFileEvent.h"

// Enable memory leak detection
#if defined(_MSC_VER) && defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
// Must be done after pre-build lirary
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _DEBUG

bool QApplication_loadTranslation(QApplication& app)
{
	bool bRes = true;

	// Load system translation
	QTranslator qtTranslator;
	qDebug("[Main] Current locale is %s", qPrintable(QLocale::system().name()));
#ifdef USE_QLIBRARYINFO_PATH
	QString szQtTranslationPath = QLibraryInfo::path(QLibraryInfo::TranslationsPath);
#else
	QString szQtTranslationPath = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
#endif
	qDebug("[Main] Current locale path is %s", qPrintable(szQtTranslationPath));
	bRes = qtTranslator.load("qt_" + QLocale::system().name(), szQtTranslationPath) && bRes;
	if(bRes){
		app.installTranslator(&qtTranslator);
	}else{
		qWarning("[Main] Failed to load qt translation");
	}

	// Load application translation
	QTranslator appTranslator;
	bRes = appTranslator.load(QLocale::system().name(), ":/ts/") && bRes;
	if(appTranslator.load(QLocale::system().name(), ":/ts/")) {
		app.installTranslator(&appTranslator);
	}else{
		qWarning("[Main] Failed to load application translation");
	}

	return bRes;
}


int main(int argc, char *argv[])
{
#if defined(_MSC_VER) && defined(_DEBUG)
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
	bool bGoOn = true;

	// Load argument
	bool bShowUsage = false;
	QString szFileToLoad;
	int i = 0;
	for(i=1; i<argc; i++){
		if(strcmp(argv[i], "--help") == 0){
			bShowUsage = true;
		}else{
			szFileToLoad = argv[i];
		}
	}

	if(bShowUsage) {
		qDebug("Usage: " APPLICATION_NAME " [OPTION...] [FILENAME]");
		qDebug("Options:");
		qDebug("  --help                Show this help");
		return -1;
	}

	// Initialize the QT application
	QApplication app(argc, argv);
	QApplication::setApplicationName(APPLICATION_NAME);
	QApplication::setApplicationVersion(APPLICATION_VERSION);
	QApplication::setOrganizationName(APPLICATION_VENDOR_NAME);
	QApplication::setOrganizationDomain(APPLICATION_VENDOR_DOMAIN);
	QApplication::setWindowIcon(QIcon(":/" APPLICATION_PACKAGE_NAME ".png"));

	qDebug("[Main] Starting application");

	QStringList listPathsTheme;
	listPathsTheme = QIcon::themeSearchPaths();
#ifdef QT_DEBUG
	listPathsTheme.append("./data/theme/icons"); // Add from the working directory
	listPathsTheme.append(app.applicationDirPath() + "/data/theme/icons"); // Add from the binary directory
#if defined(APPLE)
	listPathsTheme.append(app.applicationDirPath() + "/../data/theme/icons"); // Add from the binary directory
#endif
#endif

#if defined(WIN32)
	listPathsTheme.append("./icons_themes"); // Add from the working directory
	listPathsTheme.append(app.applicationDirPath() + "/icons_themes");
#elif defined(APPLE)
	listPathsTheme.append(app.applicationDirPath() + "/../Resources/icons_themes"); // Add from bundle
#else
	listPathsTheme.append(app.applicationDirPath() + "/../share/" + APPLICATION_PACKAGE_NAME + "/icons_themes"); // Add from the binary directory
#endif
	QIcon::setThemeSearchPaths(listPathsTheme);
	qDebug("[Main] Theme paths: %s", qPrintable(QIcon::themeSearchPaths().join(";")));
	//QIconThemeFallback::setThemeName("app_default");
	//qDebug("[Main] Using theme: %s, fallback theme: %s", qPrintable(QIcon::themeName()), qPrintable(QIconThemeFallback::themeName()));

	// Initialize translation
	QApplication_loadTranslation(app);

	//QSettingsManager::getInstance().loadConfigSettings();

	// Init GUI
	QWindowMain* pWindowMain = NULL;
	QWindowMainController controller;
	QGUIEventFilter eventFilterGUI;
	if(bGoOn){
		qDebug("[Main] Initializing GUI");
		pWindowMain = new QWindowMain();
		controller.init(pWindowMain);

		QCoreApplication::instance()->installEventFilter(&eventFilterGUI);
		eventFilterGUI.setWindowMainController(&controller);
	}

	// Show GUI
	if(bGoOn && pWindowMain){
		qDebug("[Main] Show application");
		//if (ApplicationSettings::getWindowMaximized()){
			pWindowMain->showMaximized();
		//}else{
			//pWindowMain->show();
		//}
#ifdef WIN32
		pWindowMain->setWindowIcon(QIcon(":/" APPLICATION_PACKAGE_NAME ".png"));
#else
		pWindowMain->setWindowIconText(APPLICATION_PACKAGE_NAME);
#endif
	}

	// Send auto load fil
	if(!szFileToLoad.isEmpty()){
		qDebug("[Main] Notify to open file %s", qPrintable(szFileToLoad));
		QOpenFileEvent* pEvent = new QOpenFileEvent(szFileToLoad);
		QCoreApplication::postEvent(QCoreApplication::instance(), pEvent);
	}

	// Run the event loop
	if(bGoOn && pWindowMain){
		qDebug("[Main] Starting the main loop");
		app.exec();
		qDebug("[Main] Exiting the main loop");
	}else{
		QMessageBox msgBox(QMessageBox::Critical, QT_TRANSLATE_NOOP("Main", "Error"), QT_TRANSLATE_NOOP("Main", "Error to initialize application"), QMessageBox::Ok);
		msgBox.exec();
	}

	// Delete the UI
	if(pWindowMain){
		delete pWindowMain;
		pWindowMain = NULL;
	}

	qDebug("[Main] Shutdown application");

	return 0;
}
