#include "EmulatorController.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QKeyEvent>
#include <QMap>
#include <QProcess>
#include <QSettings>

#include "coreinfo.h"
#include "movie-bk2.h"

#include "zlib.h"

QSettings EmulatorController::s_settings;
QString EmulatorController::s_path;

EmulatorController::EmulatorController(QObject* parent)
	: QObject(parent) {
	initCorePath();
	QDir coreDir(QString::fromStdString(Retro::corePath()));
	coreDir.setNameFilters(QStringList{ "*.json" });
	for (const auto& info : coreDir.entryList()) {
		QFile jsonDoc(coreDir.filePath(info));
		jsonDoc.open(QIODevice::ReadOnly);
		QByteArray json(jsonDoc.readAll());
		Retro::loadCoreInfo(QString::fromUtf8(json).toStdString());
	}
	m_scheduler.setInterval(16);
	connect(&m_scheduler, &QTimer::timeout, this, &EmulatorController::runOnce);
}

void EmulatorController::initCorePath() {
	QString appPath = QCoreApplication::applicationDirPath();
	QString hint;
#ifdef Q_OS_MAC
	hint = QString("%1/../PlugIns").arg(appPath);
#else
	hint = QString("%1/retro").arg(appPath);
#endif
	Retro::corePath(hint.toStdString()); // Set hint
}

bool EmulatorController::loadGame(const QString& path) {
	QFileInfo info(path);
	QString core = coreForFile(path);
	if (!core.size()) {
		return false;
	}

	m_re.unloadCore();
	m_initialState.clear();
	if (!m_re.loadRom(path.toStdString())) {
		return false;
	}

	m_data = std::make_unique<Retro::GameData>();
	m_scen = std::make_unique<Retro::Scenario>(*m_data);

	m_keybinds[0].clear();
	for (const auto& key : m_re.keybinds()) {
		if (!key.size()) {
			m_keybinds[0].append(-1);
		} else if (key == "ENTER") {
			m_keybinds[0].append(Qt::Key_Return); // Qt and pyglet have different names for this key
		} else {
			m_keybinds[0].append(QKeySequence(QString::fromStdString(key))[0]);
		}
	}

	for (unsigned p = 1; p < players(); ++p) {
		for (unsigned i = 0; i < m_re.buttons().size(); ++i) {
			m_keybinds[p].append(-1);
		}
	}
	s_settings.beginGroup(QString("bindings/%2").arg(platform()));
	const auto& keys = s_settings.childKeys();
	for (const auto& key : keys) {
		QVariant value = s_settings.value(key);
		if (value.isNull()) {
			continue;
		}
		int player = 0;
		QStringList keyparts = key.split('-');
		if (keyparts.size() >= 2) {
			player = keyparts.back().toUInt() - 1;
		}
		if (player >= 0) {
			setKeyBind(keyparts[0], value.toInt(), player);
		}
	}
	s_settings.endGroup();

	QDir dir = info.dir();

	m_dataManifest.clear();
	if (dir.exists("data.json")) {
		m_dataManifest = dir.filePath("data.json");
	}
	m_scenManifest.clear();
	if (dir.exists("scenario.json")) {
		m_scenManifest = dir.filePath("scenario.json");
	}

	m_data->reset();
	m_re.configureData(m_data.get());
	if (!m_dataManifest.isNull()) {
		m_data->load(m_dataManifest.toStdString());
	}

	m_scen->reset();
	if (!m_scenManifest.isNull()) {
		m_scen->load(m_scenManifest.toStdString());
	}

	stopMovie();
	emit gameChanged(path);

	return true;
}

QString EmulatorController::platform() const {
	return QString::fromStdString(m_re.core());
}

unsigned EmulatorController::players() const {
	return Retro::MAX_PLAYERS;
}

QStringList EmulatorController::keys() const {
	QStringList keys;
	for (const auto& key : m_re.buttons()) {
		if (key.size()) {
			keys.append(QString::fromStdString(key));
		}
	}
	return keys;
}

QMap<QString, int> EmulatorController::keyBinds(unsigned player) const {
	QMap<QString, int> keys;
	int k = 0;
	for (const auto& key : m_re.buttons()) {
		if (key.size()) {
			keys[QString::fromStdString(key)] = m_keybinds[player][k];
		}
		++k;
	}
	return keys;
}

void EmulatorController::setKeyBind(const QString& key, int binding, unsigned player) {
	const auto& bindings = m_re.buttons();
	const auto& iter = std::find(bindings.begin(), bindings.end(), key.toStdString());
	if (iter != bindings.end()) {
		m_keybinds[player][iter - bindings.begin()] = binding;
		if (player > 0) {
			s_settings.setValue(QString("bindings/%1/%2-%3").arg(platform()).arg(key).arg(player + 1), binding);
		} else {
			s_settings.setValue(QString("bindings/%1/%2").arg(platform()).arg(key), binding);
		}
	}
	s_settings.sync();
}

Retro::GameData* EmulatorController::data() {
	if (!m_running) {
		return nullptr;
	}
	return m_data.get();
}

Retro::Scenario* EmulatorController::scenario() {
	if (!m_running) {
		return nullptr;
	}
	return m_scen.get();
}

void EmulatorController::setDataManifest(const QString& file) {
	m_dataManifest = file;
}

void EmulatorController::setScenarioManifest(const QString& file) {
	m_scenManifest = file;
}

void EmulatorController::setSearchFile(const QString& file) {
	m_searchFile = file;
}

void EmulatorController::loadState(const QByteArray& data) {
	m_initialState.clear();
	stopMovie();
	QByteArray buffer(2048, 0);
	z_stream zs{};

	zs.avail_in = data.size();
	zs.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(data.constData()));
	inflateInit2(&zs, 31);
	zs.avail_out = buffer.size();
	zs.next_out = reinterpret_cast<Bytef*>(buffer.data());
	int err = inflate(&zs, 0);
	do {
		buffer.resize(buffer.size() - zs.avail_out);
		m_initialState.append(buffer);
		zs.avail_out = buffer.size();
		zs.next_out = reinterpret_cast<Bytef*>(buffer.data());
		int err = inflate(&zs, 0);
		if (err < 0) {
			break;
		}
	} while (err != Z_STREAM_END && zs.avail_out != buffer.size());
	inflateEnd(&zs);

	if (m_running) {
		start();
	}
}

void EmulatorController::clearState() {
	m_initialState.clear();
}

bool EmulatorController::loadMovie(const QString& path) {
	std::unique_ptr<Retro::Movie> movie = Retro::Movie::load(path.toStdString());
	if (!movie) {
		return false;
	}
	std::vector<uint8_t> state;
	if (movie->getState(&state)) {
		m_initialState = QByteArray(reinterpret_cast<const char*>(state.data()), state.size());
	} else {
		m_initialState.clear();
	}
	QString gameName = QString::fromStdString(movie->getGameName());
	if (m_running && (gameName.isEmpty() || gameName == "?")) {
		start();
	} else {
		QStringList paths{ "stable", "experimental", "contrib" };
		bool found = false;
		for (const auto& p : paths) {
			QDir path(dataPath());
			path.cd(p);
			path.cd(gameName);
			QStringList roms = path.entryList({ "rom.*" });
			roms.removeAll("rom.sha");
			if (roms.empty()) {
				continue;
			}
			QByteArray state = m_initialState;
			if (loadGame(path.filePath(roms[0]))) {
				m_initialState = state;
				start();
				found = true;
				emit gameChanged(path.absolutePath());
				break;
			}
		}
		if (!found) {
			return false;
		}
	}
	m_movie = std::move(movie);
	m_recording = false;
	return true;
}

bool EmulatorController::recordMovie(const QString& path, const QString& gamename) {
	std::unique_ptr<Retro::MovieBK2> movie = std::make_unique<Retro::MovieBK2>(path.toStdString(), true);
	movie->loadKeymap(m_re.core());
	if (!gamename.isNull()) {
		movie->setGameName(gamename.toStdString());
	}
	if (m_running) {
		start();
	}
	m_movie = move(movie);
	m_movie->setState(reinterpret_cast<const uint8_t*>(m_initialState.constData()), m_initialState.size());
	m_recording = true;
	return true;
}

QByteArray EmulatorController::saveState() {
	if (!m_running) {
		return QByteArray();
	}

	QByteArray data(m_re.serializeSize(), 0);
	m_re.serialize(static_cast<void*>(data.data()), data.size());
	QByteArray buffer;
	z_stream zs{};

	zs.avail_in = data.size();
	zs.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(data.constData()));
	deflateInit2(&zs, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 31, 8, Z_DEFAULT_STRATEGY);
	int err;
	do {
		zs.avail_out = 2048;
		buffer.resize(buffer.size() + zs.avail_out);
		zs.next_out = reinterpret_cast<Bytef*>(&buffer.data()[buffer.size() - zs.avail_out]);
		err = deflate(&zs, Z_FINISH);
		if (err < 0) {
			break;
		}
	} while (!zs.avail_out && err != Z_STREAM_END);
	deflateEnd(&zs);

	if (zs.avail_out > 0) {
		buffer.resize(buffer.size() - zs.avail_out);
	}
	return buffer;
}

QImage EmulatorController::screen() const {
	return m_screen;
}

QString EmulatorController::coreForFile(const QString& path) {
	return QString::fromStdString(Retro::coreForRom(path.toStdString()));
}

QString EmulatorController::dataPath() {
	if (s_path.isNull() || !QDir(s_path).exists()) {
		s_path = s_settings.value("paths/data").toString();
	}
	if (s_path.isNull() || !QDir(s_path).exists()) {
		s_path = QString::fromStdString(Retro::GameData::dataPath());
	}
	if (s_path.isNull() || !QDir(s_path).exists()) {
		QProcess* subproc = new QProcess;
		connect(subproc, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), subproc, [subproc](int, QProcess::ExitStatus) {
			subproc->deleteLater();
		});
		subproc->start("python3", {"-c", "import retro; print(retro.data.path())"});
		subproc->waitForStarted();
		while (subproc->state() != QProcess::NotRunning) {
			subproc->waitForFinished(10);
			if (subproc->canReadLine()) {
				s_path = QString::fromUtf8(subproc->readLine()).trimmed();
				break;
			}
		}
		if (s_path.isEmpty() || subproc->exitStatus() != QProcess::NormalExit || subproc->exitCode() != 0) {
			s_path = QString();
		}
	}
	return s_path;
}

QList<Cheat> EmulatorController::cheats() const {
	return m_cheats;
}

void EmulatorController::start() {
	m_re.reset();
	m_screen = QImage();
	stopMovie();
	m_re.unserialize(static_cast<void*>(m_initialState.data()), m_initialState.size());
	m_data->updateRam();
	m_data->updateRam();
	m_scen->restart();
	m_running = true;
	schedule();
	emit started();
}

void EmulatorController::stop() {
	m_running = false;
	m_scheduler.stop();
}

void EmulatorController::schedule() {
	if (!m_paused) {
		m_scheduler.start();
	}
}

void EmulatorController::setFastForward(bool ff) {
	m_fastForward = ff;
	if (ff) {
		m_scheduler.setInterval(1);
	} else {
		m_scheduler.setInterval(16);
	}
}

void EmulatorController::setPaused(bool pause) {
	if (pause == m_paused) {
		return;
	}
	m_paused = pause;
	if (pause) {
		m_scheduler.stop();
	} else {
		schedule();
	}
	emit pauseChanged(pause);
}

void EmulatorController::stopMovie() {
	m_movie.reset();
	m_recording = false;
	for (int i = 0; i < 16; ++i) {
		m_re.setKey(0, i, false);
	}
	emit movieStopped();
}

void EmulatorController::variablesUpdated() {
	if (m_autosaveData && !m_dataManifest.isNull()) {
		m_data->save(m_dataManifest.toStdString());
	}
}

void EmulatorController::scenarioUpdated() {
	if (m_autosaveScen && !m_scenManifest.isNull()) {
		m_scen->save(m_scenManifest.toStdString());
	}
}

void EmulatorController::searchesUpdated() {
#ifdef USE_CAPNP
	if (m_autosaveSearches && !m_searchFile.isNull()) {
		m_data->saveSearches(m_searchFile.toStdString());
	}
#endif
}

void EmulatorController::setAutosaveVariables(bool autosave) {
	m_autosaveData = autosave;
}

void EmulatorController::setAutosaveScenario(bool autosave) {
	m_autosaveScen = autosave;
}

void EmulatorController::setAutosaveSearches(bool autosave) {
	m_autosaveSearches = autosave;
}

void EmulatorController::addCheat(const QString& name, const QString& value, bool enabled) {
	addCheat(Cheat{ name, value, enabled });
}

void EmulatorController::addCheat(const Cheat& cheat) {
	m_cheats.append(cheat);
	if (!cheat.enabled) {
		return;
	}
	reloadCheats();
}

void EmulatorController::enableCheat(unsigned index, bool enabled) {
	m_cheats[index].enabled = enabled;
	reloadCheats();
}

void EmulatorController::removeCheat(unsigned index) {
	m_cheats.removeAt(index);
	reloadCheats();
}

void EmulatorController::clearCheats() {
	m_cheats.clear();
	m_re.clearCheats();
}

void EmulatorController::step() {
	if (!m_paused) {
		m_paused = true;
		emit pauseChanged(true);
	}
	m_scheduler.stop();
	runOnce();
}

void EmulatorController::runOnce() {
	if (!m_running) {
		return;
	}
	runOneStep();
	if (m_fastForward && !m_paused) {
		runOneStep();
		runOneStep();
		runOneStep();
	}
	size_t x;
	size_t y;
	size_t width;
	size_t height;
	m_scen->getCrop(&x, &y, &width, &height);
	QRect crop{
		static_cast<int>(x),
		static_cast<int>(y),
		static_cast<int>(width),
		static_cast<int>(height)
	};

	if (m_screen.constBits() != static_cast<const uchar*>(m_re.getImageData()) || m_crop != crop) {
		QImage::Format format = QImage::Format_RGB16;
		switch (m_re.getImageDepth()) {
		case 32:
			format = QImage::Format_RGB32;
			break;
		case 16:
			format = QImage::Format_RGB16;
			break;
		case 15:
			format = QImage::Format_RGB555;
			break;
		}
		m_screen = QImage(static_cast<const uchar*>(m_re.getImageData()), m_re.getImageWidth(), m_re.getImageHeight(), m_re.getImagePitch(), format);
		m_crop = crop;
		if (crop.x() + crop.width() > m_re.getImageWidth() || crop.width() == 0) {
			crop.setWidth(m_re.getImageWidth() - x);
		}
		if (crop.y() + crop.height() > m_re.getImageHeight() || crop.height() == 0) {
			crop.setHeight(m_re.getImageHeight() - y);
		}
		m_screen = m_screen.copy(crop);
	}

	emit frameAvailable(m_screen);
}

void EmulatorController::runOneStep() {
	if (m_movie) {
		if (m_recording) {
			for (int i = 0; i < 16; ++i) {
				m_movie->setKey(i, m_re.getKey(0, i));
			}
		}
		if (m_movie->step()) {
			if (!m_recording) {
				for (unsigned p = 0; p < m_movie->players(); ++p) {
					for (int i = 0; i < 16; ++i) {
						m_re.setKey(p, i, m_movie->getKey(i, p));
					}
				}
			}
		} else {
			if (!m_recording) {
				setPaused(true);
			}
			stopMovie();
		}
	}
	m_re.run();
	m_data->updateRam();
	emit dataUpdated();
}

void EmulatorController::reloadCheats() {
	m_re.clearCheats();
	unsigned i = 0;
	for (const auto& cheat : m_cheats) {
		if (!cheat.enabled) {
			continue;
		}
		m_re.setCheat(i, true, cheat.value.toUtf8().constData());
		++i;
	}
}

bool EmulatorController::eventFilter(QObject*, QEvent* event) {
	if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) {
		QKeyEvent* key = static_cast<QKeyEvent*>(event);
		bool down = key->type() == QEvent::KeyPress;
		if (key->key() == Qt::Key_Space) {
			setFastForward(down);
			return true;
		}
		if (!m_movie || m_recording) {
			for (unsigned p = 0; p < players(); ++p) {
				int button = m_keybinds[p].indexOf(key->key());
				if (button >= 0) {
					m_re.setKey(p, button, down);
				}
			}
		}
		return true;
	}
	return false;
}
