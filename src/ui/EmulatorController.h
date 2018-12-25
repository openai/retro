#pragma once

#include <QImage>
#include <QMap>
#include <QObject>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QTimer>

#include <functional>

#include "data.h"
#include "emulator.h"
#include "movie.h"

struct Cheat {
	QString name;
	QString value;
	bool enabled;
};

class EmulatorController : public QObject {
	Q_OBJECT
public:
	EmulatorController(QObject* parent = nullptr);

	bool loadGame(const QString&);

	static QStringList extensions();

	QString platform() const;
	unsigned players() const;

	QStringList keys() const;
	QMap<QString, int> keyBinds(unsigned player = 0) const;
	void setKeyBind(const QString&, int, unsigned player = 0);

	Retro::GameData* data();
	Retro::Scenario* scenario();

	QByteArray saveState();
	void loadState(const QByteArray&);
	void clearState();

	bool loadMovie(const QString&);
	bool recordMovie(const QString& path, const QString& gamename = {});

	QImage screen() const;

	static QString coreForFile(const QString&);
	static QString dataPath();

	QList<Cheat> cheats() const;

signals:
	void started();
	void pauseChanged(bool);
	void frameAvailable(const QImage&);
	void dataUpdated();
	void searchUpdated();
	void movieStopped();
	void gameChanged(const QString& path);

public slots:
	void start();
	void stop();
	void schedule();
	void step();

	void setFastForward(bool);
	void setPaused(bool);

	void stopMovie();

	void setDataManifest(const QString&);
	void setScenarioManifest(const QString&);
	void setSearchFile(const QString&);

	void variablesUpdated();
	void scenarioUpdated();
	void searchesUpdated();

	void setAutosaveVariables(bool);
	void setAutosaveScenario(bool);
	void setAutosaveSearches(bool);

	void addCheat(const QString& name, const QString& value, bool enabled = true);
	void addCheat(const Cheat& cheat);
	void enableCheat(unsigned index, bool enabled = true);
	void removeCheat(unsigned index);
	void clearCheats();

private slots:
	void runOnce();
	void reloadCheats();

protected:
	bool eventFilter(QObject* obj, QEvent* event) override;

private:
	void initCorePath();
	void runOneStep();

	Retro::Emulator m_re;
	std::unique_ptr<Retro::GameData> m_data;
	std::unique_ptr<Retro::Scenario> m_scen;
	QByteArray m_initialState;
	std::unique_ptr<Retro::Movie> m_movie;
	bool m_recording = false;

	QImage m_screen;
	QRect m_crop;
	QTimer m_scheduler;
	QList<int> m_keybinds[Retro::MAX_PLAYERS];
	bool m_running = false;
	bool m_paused = false;
	bool m_fastForward = false;

	QList<Cheat> m_cheats;

	QString m_dataManifest;
	QString m_scenManifest;
	QString m_searchFile;
	bool m_autosaveData = false;
	bool m_autosaveScen = false;
	bool m_autosaveSearches = false;

	static QSettings s_settings;
	static QString s_path;
};
