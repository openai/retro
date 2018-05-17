#pragma once

#include <QMainWindow>
#include <QSettings>
#include <QTimer>

#include "CheatModel.h"
#include "GameDataModel.h"
#include "SearchResultsModel.h"
#include "SearchModel.h"

namespace Retro {
class GameData;
}

namespace Ui {
class MainWindow;
}

class EmulatorController;
class Screen;

class MainWindow : public QMainWindow {
	Q_OBJECT
public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

	Screen* screen();

	void setEmulator(EmulatorController*);
	void setData(Retro::GameData*);

public slots:
	void openAny(const QString&);
	void loadState(const QString&);

private slots:
	void updateScenario();
	void resetScenario();
	void performSearch();
	void updateSearchResults(const QModelIndex&);
	void removeSearch();
	void migrateSearch();

	void addVariable();
	void removeVariable();

	void addCheat();
	void removeCheat();

	void integrate(const QString& before, const QString& after, const QString& extension);
	void startGame(const QString& path);
	void changeGame(const QString& path);
	void startEpisode();

private:
	Ui::MainWindow* m_ui;

	EmulatorController* m_controller = nullptr;
	GameDataModel m_dataModel;
	SearchModel m_searchModel;
	SearchResultsModel m_searchResultsModel;
	CheatModel m_cheatModel;

	bool m_didEnd = false;
	QString m_gameDir;
	QString m_integrateDir;
	QString m_recordDir;
	QSettings m_settings;
	QTimer m_searchUpdateTimer;
	bool m_recordingMode;
	QString m_gameName;
	QString m_stateName;
};
