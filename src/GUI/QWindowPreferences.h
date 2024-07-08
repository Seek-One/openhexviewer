#ifndef SRC_PREFERENCESWINDOW_H_
#define SRC_PREFERENCESWINDOW_H_

#include <QMainWindow>

#include "QPreferencesFilesStructuresView.h"

class QAction;
class QPreferencesFilesStructuresView;

/**
 * @brief The preferences window class.
 * 
 * The QWindowPreferences class represents the main preferences window
 * for managing file structures and settings.
 */
class QWindowPreferences : public QMainWindow
{
	Q_OBJECT
public:
	/**
     * @brief Constructs a new QWindowPreferences object.
     * 
     * @param parent The parent widget of the QWindowPreferences. Defaults to nullptr.
     */
	QWindowPreferences(QWidget* parent = 0);

	/**
     * @brief Destroys the QWindowPreferences object.
     */
	virtual ~QWindowPreferences();

	/**
     * @brief Gets the file structure view widget.
     * 
     * @return A pointer to the QPreferencesFilesStructuresView object.
     */
    QPreferencesFilesStructuresView* getFileStructureView() const;

public slots:
	/**
     * @brief Slot function to update the status bar text.
     * 
     * @param szText The text to display in the status bar.
     */
	void changedPreferencesStatusBar(QString szText);


private:
	/**
     * @brief Creates the status bar for the preferences window.
     */
    void createStatusBar();
	
    QPreferencesFilesStructuresView* m_pPreferencesFilesStructuresView; /**< File structures view widget. */
	QStatusBar* m_pStatusBar; /**< Status bar widget. */
};

#endif /* SRC_PREFERENCESWINDOW_H_ */
