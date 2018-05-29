#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QCryptographicHash>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QRegularExpression>

#include "coreinfo.h"

#include "ControlsWidget.h"
#include "EmulatorController.h"
#include "ScenarioEditor.h"

using M = Retro::Scenario::Measurement;
using O = Retro::Operation;

// NB: Keep in sync with list in MainWindow.ui
static const QList<QPair<M, O>> s_searchTypes{
	qMakePair(M::ABSOLUTE, O::EQUAL),
	qMakePair(M::DELTA, O::EQUAL),
	qMakePair(M::DELTA, O::NEGATIVE_EQUAL),
	qMakePair(M::DELTA, O::POSITIVE),
	qMakePair(M::DELTA, O::NEGATIVE),
	qMakePair(M::DELTA, O::NOT_EQUAL),
	qMakePair(M::DELTA, O::EQUAL),
};

static const QList<bool> s_searchReference{
	true, true, true, false, false, false, false
};

static QString s_extensionString;
static QString s_allExtensionString;

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, m_ui(new Ui::MainWindow) {
	m_ui->setupUi(this);

	QDir dataDir(EmulatorController::dataPath());
	if (!dataDir.exists()) {
		QMessageBox* warning = new QMessageBox(QMessageBox::Warning, tr("Data directory missing"),
			tr("Could not find data directory. Integration and movie loading will not work properly."),
			QMessageBox::Ok);
		warning->setAttribute(Qt::WA_DeleteOnClose);
		warning->show();
	}

	if (s_extensionString.isEmpty()) {
		s_extensionString = tr("ROMs");
		QStringList extensionStrings;
		for (const auto& extension : Retro::extensions()) {
			extensionStrings.append(QString("*.%1").arg(QString::fromStdString(extension)));
		}
		s_extensionString.append(" (");
		s_extensionString.append(extensionStrings.join(" "));
		s_extensionString.append(")");
	}
	if (s_allExtensionString.isEmpty()) {
		s_allExtensionString = tr("ROMs/movies/states");
		QStringList extensionStrings;
		for (const auto& extension : Retro::extensions()) {
			extensionStrings.append(QString("*.%1").arg(QString::fromStdString(extension)));
		}
		extensionStrings.append("*.state");
		extensionStrings.append("*.fm2");
		extensionStrings.append("*.bk2");
		s_allExtensionString.append(" (");
		s_allExtensionString.append(extensionStrings.join(" "));
		s_allExtensionString.append(")");
	}

	m_integrateDir = m_settings.value("paths/integrate_dir").toString();
	m_recordingMode = m_settings.value("mode/record").toBool();
	m_ui->actionEpisodeRecording->setChecked(m_recordingMode);

	Q_ASSERT(s_searchTypes.size() == s_searchReference.size());
	Q_ASSERT(s_searchTypes.size() == m_ui->searchType->count());

	connect(m_ui->actionOpen, &QAction::triggered, [this]() {
		QString rom = QFileDialog::getOpenFileName(this, tr("Select ROM"), QString(), s_extensionString);
		if (!rom.isNull() && m_controller->loadGame(rom)) {
			startGame(rom);
		}
	});

	connect(m_ui->actionOpenAny, &QAction::triggered, [this]() {
		QString file = QFileDialog::getOpenFileName(this, tr("Select file"), QString(), s_allExtensionString);
		if (file.isNull()) {
			return;
		}
		openAny(file);
	});

	connect(m_ui->actionSaveState, &QAction::triggered, [this]() {
		QString state = QFileDialog::getSaveFileName(this, tr("State name"), m_gameDir, tr("State file (*.state)"));
		if (state.isNull()) {
			return;
		}
		QFile file(state);
		file.open(QIODevice::WriteOnly);
		QByteArray data = m_controller->saveState();
		file.write(data);
	});

	connect(m_ui->actionLoadState, &QAction::triggered, [this]() {
		QString state = QFileDialog::getOpenFileName(this, tr("State name"), m_gameDir, tr("State file (*.state)"));
		if (state.isNull()) {
			return;
		}
		loadState(state);
	});

	connect(m_ui->actionScreenshot, &QAction::triggered, [this]() {
		QString png = QFileDialog::getSaveFileName(this, tr("Screenshot name"), m_gameDir, tr("Portable Network Graphic (*.png)"));
		if (png.isNull()) {
			return;
		}
		QImage screen = m_controller->screen();
		screen.save(png);
	});

	connect(m_ui->actionLoadVars, &QAction::triggered, [this]() {
		QString manifest = QFileDialog::getOpenFileName(this, tr("Select data.json"), m_gameDir, tr("JSON file (*.json)"));
		if (!manifest.isNull()) {
			m_controller->data()->load(manifest.toStdString());
			m_controller->setDataManifest(manifest);
			setData(m_controller->data());
		}
	});

	connect(m_ui->actionSaveVars, &QAction::triggered, [this]() {
		QString manifest = QFileDialog::getSaveFileName(this, tr("Select data.json"), m_gameDir, tr("JSON file (*.json)"));
		if (!manifest.isNull()) {
			m_controller->data()->save(manifest.toStdString());
			m_controller->setDataManifest(manifest);
		}
	});

#ifdef USE_CAPNP
	connect(m_ui->actionLoadSearch, &QAction::triggered, [this]() {
		QString manifest = QFileDialog::getOpenFileName(this, tr("Select search"), m_gameDir, tr("Search file (*.search)"));
		if (!manifest.isNull()) {
			m_controller->data()->loadSearches(manifest.toStdString());
			m_controller->setSearchFile(manifest);
			m_searchModel.refresh();
			m_searchResultsModel.setVariable(QString());
		}
	});

	connect(m_ui->actionSaveSearch, &QAction::triggered, [this]() {
		QString manifest = QFileDialog::getSaveFileName(this, tr("Search data.json"), m_gameDir, tr("Search file (*.search)"));
		if (!manifest.isNull()) {
			m_controller->data()->saveSearches(manifest.toStdString());
			m_controller->setSearchFile(manifest);
		}
	});
#else
	m_ui->actionLoadSearch->setVisible(false);
	m_ui->actionSaveSearch->setVisible(false);
	m_ui->actionAutosaveSearches->setVisible(false);
#endif

	connect(m_ui->actionLoadScenario, &QAction::triggered, [this]() {
		QString manifest = QFileDialog::getOpenFileName(this, tr("Select scenario.json"), m_gameDir, tr("JSON file (*.json)"));
		if (!manifest.isNull()) {
			m_controller->scenario()->load(manifest.toStdString());
			m_controller->setScenarioManifest(manifest);
		}
	});

	connect(m_ui->actionEditScenario, &QAction::triggered, [this]() {
		ScenarioEditor* editor = new ScenarioEditor;
		editor->setAttribute(Qt::WA_DeleteOnClose);
		editor->setScenario(m_controller->scenario());
		connect(editor, &ScenarioEditor::scenarioSaved, m_controller, &EmulatorController::setScenarioManifest);
		connect(editor, &ScenarioEditor::scenarioUpdated, m_controller, &EmulatorController::scenarioUpdated);
		connect(m_controller, &EmulatorController::started, editor, [this, editor]() {
			editor->setScenario(m_controller->scenario());
		});
		editor->show();
	});

	connect(m_ui->actionIntegrate, &QAction::triggered, [this]() {
		QString file = QFileDialog::getOpenFileName(this, tr("Select ROM"), m_integrateDir, s_extensionString);
		if (file.isNull()) {
			return;
		}
		QString core = EmulatorController::coreForFile(file);
		if (!core.isNull()) {
			QFileInfo fileinfo(file);
			QString oldName = fileinfo.completeBaseName();
			m_integrateDir = fileinfo.dir().absolutePath();
			m_settings.setValue("paths/integrate_dir", m_integrateDir);
			m_settings.sync();
			QString name;
			int oldStart = 0;
			// Convert to CamelCase
			auto matches = QRegularExpression("(?:^|[-._~ ])([a-z]?)").globalMatch(oldName);
			while (matches.hasNext()) {
				auto match = matches.next();
				name.append(oldName.mid(oldStart, match.capturedStart() - oldStart));
				name.append(match.captured(1).toUpper());
				oldStart = match.capturedEnd();
			}
			name.append(oldName.mid(oldStart));
			name.remove(QRegularExpression("[\\[(]!p?[\\])]")); // Remove goodtools tags
			name.remove(QRegularExpression("\\([ABCEFGIJRSUW]{1,3}\\)")); // Remove region info
			name.remove(QRegularExpression("\\((Asia|Australia|Brazil|Canada|China|HongKong|Netherlands|Europe|France|Germany|Italy|Japan|Korea|Russia|Spain|Sweden|USA|World|,)+\\)")); // Remove region info
			name.remove(QRegularExpression("\\((En|Rev[0-9A-F.]+)\\)")); // Remove language and revision info
			name.remove(",The"); // Remove trailing 'the'
			name.remove(QRegularExpression("[()\\[\\]'!+,%^;]")); // Remove extra punctuation
			name.replace(QRegularExpression("&"), "And");
			name.replace(QRegularExpression("\\$"), "s");
			name.append("-" + core);
			bool ok;
			QString text = QInputDialog::getText(this, tr("Destination name"),
				tr("Please enter game name:"), QLineEdit::Normal, name, &ok);
			if (!text.isNull() && ok) {
				integrate(file, text, fileinfo.suffix());
			}
		}
	});

	connect(m_ui->actionLoadExisting, &QAction::triggered, [this]() {
		QString game = QFileDialog::getExistingDirectory(this, tr("Select game"), EmulatorController::dataPath());
		if (game.isNull()) {
			return;
		}
		QDir path(game);
		QStringList roms = path.entryList({ "rom.*" });
		roms.removeAll("rom.sha");
		if (roms.empty()) {
			return;
		}
		if (m_controller->loadGame(path.filePath(roms[0]))) {
			startGame(game);
		}
	});

	connect(m_ui->actionPlayMovie, &QAction::triggered, [this]() {
		QString movie = QFileDialog::getOpenFileName(this, tr("Movie file"), QString(), tr("Movie (*.bk2 *.fm2)"));
		if (movie.isNull()) {
			return;
		}
		if (m_controller->loadMovie(movie)) {
			m_ui->movie->setText(tr("Playing"));
			resetScenario();
		}
	});

	connect(m_ui->actionRecordMovie, &QAction::triggered, [this]() {
		QString movie = QFileDialog::getSaveFileName(this, tr("Movie file"), QString(), tr("Movie (*.bk2)"));
		if (movie.isNull()) {
			return;
		}
		if (m_controller->recordMovie(movie, m_gameName)) {
			m_ui->movie->setText(tr("Recording"));
			resetScenario();
		}
	});

	connect(m_ui->actionEpisodeRecording, &QAction::triggered, [this](bool active) {
		m_recordingMode = active;
		m_settings.setValue("mode/record", m_recordingMode);
		m_settings.sync();
		if (m_recordingMode) {
			startEpisode();
		}
	});

	connect(m_ui->actionSetDataDirectory, &QAction::triggered, [this]() {
		QString dir = QFileDialog::getExistingDirectory(this, tr("Select directory"), EmulatorController::dataPath());
		if (dir.isNull()) {
			return;
		}
		m_settings.setValue("paths/data", dir);
		m_settings.sync();
	});

	connect(m_ui->actionControls, &QAction::triggered, [this]() {
		if (!m_controller) {
			return;
		}
		ControlsWidget* controls = new ControlsWidget(m_controller->platform());
		controls->setAttribute(Qt::WA_DeleteOnClose);
		controls->setKeys(m_controller->keys());
		controls->setPlayers(m_controller->players());
		for (unsigned player = 0; player < m_controller->players(); ++player) {
			controls->setBindings(m_controller->keyBinds(player), player);
		}
		connect(controls, &ControlsWidget::bindingChanged, m_controller, &EmulatorController::setKeyBind);
		controls->show();
	});

	m_ui->variables->setModel(&m_dataModel);
	m_ui->search->setModel(&m_searchModel);
	m_ui->searchResults->setModel(&m_searchResultsModel);
	m_ui->cheats->setModel(&m_cheatModel);
	m_searchUpdateTimer.setInterval(100);

	connect(m_ui->searchName, &QLineEdit::returnPressed, this, &MainWindow::performSearch);
	connect(m_ui->searchSubmit, &QAbstractButton::clicked, this, &MainWindow::performSearch);
	connect(m_ui->search, &QAbstractItemView::clicked, this, &MainWindow::updateSearchResults);
	connect(m_ui->searchRemove, &QAbstractButton::clicked, this, &MainWindow::removeSearch);
	connect(m_ui->searchType, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [this](int index) {
		m_ui->searchValue->setEnabled(s_searchReference[index]);
	});
	connect(m_ui->searchMigrate, &QAbstractButton::clicked, this, &MainWindow::migrateSearch);

	connect(m_ui->variableName, &QLineEdit::returnPressed, this, &MainWindow::addVariable);
	connect(m_ui->variableType, &QLineEdit::returnPressed, this, &MainWindow::addVariable);
	connect(m_ui->variableAdd, &QAbstractButton::clicked, this, &MainWindow::addVariable);
	connect(m_ui->variableRemove, &QAbstractButton::clicked, this, &MainWindow::removeVariable);

	connect(m_ui->resetScenario, &QAbstractButton::clicked, this, &MainWindow::resetScenario);

	connect(m_ui->cheatAdd, &QAbstractButton::pressed, this, &MainWindow::addCheat);
	connect(m_ui->cheatName, &QLineEdit::returnPressed, this, &MainWindow::addCheat);
	connect(m_ui->cheatRemove, &QAbstractButton::pressed, this, &MainWindow::removeCheat);
}

MainWindow::~MainWindow() {
	delete m_ui;
}

Screen* MainWindow::screen() {
	return m_ui->screen;
}

void MainWindow::setEmulator(EmulatorController* controller) {
	m_controller = controller;
	connect(controller, &EmulatorController::frameAvailable, screen(), &Screen::setImage);
	connect(controller, &EmulatorController::dataUpdated, &m_dataModel, &GameDataModel::refresh);
	connect(controller, &EmulatorController::dataUpdated, this, &MainWindow::updateScenario);
	connect(&m_searchUpdateTimer, &QTimer::timeout, &m_searchResultsModel, &SearchResultsModel::refresh);

	connect(controller, &EmulatorController::pauseChanged, screen(), &Screen::setPaused);
	connect(screen(), &Screen::pauseChanged, m_ui->actionPause, &QAction::setChecked);
	connect(screen(), &Screen::pauseChanged, controller, &EmulatorController::setPaused);
	connect(screen(), &Screen::pauseChanged, &m_searchUpdateTimer, [this](bool paused) {
		if (paused) {
			m_searchUpdateTimer.stop();
		} else {
			m_searchUpdateTimer.start();
		}
	});

	connect(m_ui->actionPause, &QAction::triggered, controller, &EmulatorController::setPaused);
	connect(m_ui->actionStep, &QAction::triggered, controller, &EmulatorController::step);
	connect(m_ui->actionStep, &QAction::triggered, &m_searchResultsModel, &SearchResultsModel::refresh);

	connect(m_ui->actionReset, &QAction::triggered, controller, &EmulatorController::start);
	connect(m_ui->actionReset, &QAction::triggered, controller, [this]() {
		if (m_recordingMode) {
			startEpisode();
		}
	});
	connect(m_ui->actionHardReset, &QAction::triggered, controller, [controller]() {
		controller->clearState();
		controller->start();
	});
	connect(controller, &EmulatorController::started, [this]() {
		setData(m_controller->data());
		m_ui->actionControls->setEnabled(true);
		m_searchUpdateTimer.start();
		resetScenario();
		screen()->setFocus();
	});

	connect(m_ui->actionAutosaveVariables, &QAction::toggled, m_controller, &EmulatorController::setAutosaveVariables);
	connect(m_ui->actionAutosaveScenario, &QAction::toggled, m_controller, &EmulatorController::setAutosaveScenario);
	connect(m_ui->actionAutosaveSearches, &QAction::toggled, m_controller, &EmulatorController::setAutosaveSearches);
	connect(&m_dataModel, &QAbstractItemModel::dataChanged, m_controller, &EmulatorController::variablesUpdated);

	connect(m_ui->actionStopMovie, &QAction::triggered, m_controller, &EmulatorController::stopMovie);
	connect(controller, &EmulatorController::movieStopped, [this]() {
		m_ui->movie->setText(tr("None"));
	});
	connect(controller, &EmulatorController::gameChanged, [this](const QString& path) {
		changeGame(path);
	});
	m_controller->setAutosaveVariables(m_ui->actionAutosaveVariables->isChecked());
	m_controller->setAutosaveScenario(m_ui->actionAutosaveScenario->isChecked());
	m_controller->setAutosaveSearches(m_ui->actionAutosaveSearches->isChecked());

	m_cheatModel.setEmulator(controller);
}

void MainWindow::setData(Retro::GameData* data) {
	m_ui->actionReset->setEnabled(true);
	m_ui->actionHardReset->setEnabled(true);
	m_ui->actionPause->setEnabled(true);
	m_ui->actionStep->setEnabled(true);

	m_ui->actionEditScenario->setEnabled(true);
	m_ui->actionLoadVars->setEnabled(true);
	m_ui->actionSaveVars->setEnabled(true);
#ifdef USE_CAPNP
	m_ui->actionLoadSearch->setEnabled(true);
	m_ui->actionSaveSearch->setEnabled(true);
#endif
	m_dataModel.setDataBacking(data);
	m_searchModel.setDataBacking(data);
	m_searchResultsModel.setDataBacking(data);
}

void MainWindow::openAny(const QString& file) {
	if (m_controller->loadGame(file)) {
		startGame(file);
		return;
	}
	if (m_controller->loadMovie(file)) {
		m_ui->movie->setText(tr("Playing"));
		resetScenario();
		return;
	}
	loadState(file);
}

void MainWindow::loadState(const QString& path) {
	QFile file(path);
	file.open(QIODevice::ReadOnly);
	QByteArray data = file.readAll();
	m_controller->loadState(data);
	QFileInfo info(path);
	m_stateName = info.completeBaseName();
	if (m_recordingMode) {
		startEpisode();
	}
}

void MainWindow::resetScenario() {
	if (!m_controller->scenario()) {
		return;
	}
	m_didEnd = false;
	m_controller->scenario()->reloadScripts();
	updateScenario();
}

void MainWindow::updateScenario() {
	if (!m_controller) {
		return;
	}
	Retro::Scenario* scen = m_controller->scenario();
	if (!scen) {
		return;
	}
	float reward = 0;
	float totalReward = 0;
	bool done = false;
	bool rewardError = false;
	bool doneError = false;
	try {
		scen->update();
	} catch (std::runtime_error e) {
		std::cerr << e.what() << std::endl;
		rewardError = true;
		doneError = true;
	}
	reward = scen->currentReward();
	totalReward = scen->totalReward();
	done = scen->isDone();
	if (done) {
		m_didEnd = true;
	}
	m_ui->reward->setText(rewardError ? tr("Error") : QString::number(reward));
	m_ui->cumReward->setText(QString::number(totalReward));
	m_ui->done->setText(doneError ? tr("Error") : done ? tr("Yes") : tr("No"));
	m_ui->didEnd->setText(m_didEnd ? tr("Yes") : tr("No"));
	m_ui->frame->setText(QString::number(scen->frame()));
	m_ui->ts->setText(QString::number(scen->timestep()));
	if (m_recordingMode && done) {
		startEpisode();
	}
}

void MainWindow::performSearch() {
	std::string name = m_ui->searchName->text().toStdString();
	if (!name.size() || !m_searchModel.getDataBacking()) {
		return;
	}
	const auto& searchType = s_searchTypes[m_ui->searchType->currentIndex()];
	if (searchType.first == M::ABSOLUTE) {
		m_searchModel.getDataBacking()->search(name, m_ui->searchValue->value());
	} else {
		int64_t value = s_searchReference[m_ui->searchType->currentIndex()] ? m_ui->searchValue->value() : 0;
		m_searchModel.getDataBacking()->deltaSearch(name, searchType.second, value);
	}

	const Retro::Search* search = m_searchModel.getDataBacking()->getSearch(name);
	if (search->hasUniqueResult()) {
		m_searchModel.getDataBacking()->setVariable(name, search->typedResults()[0]);
		m_controller->variablesUpdated();
		if (m_ui->searchName->text() == m_searchResultsModel.getVariable()) {
			m_searchResultsModel.setVariable(QString());
		}
		m_searchModel.getDataBacking()->removeSearch(name);
		m_dataModel.refresh();
	}

	m_controller->searchesUpdated();
	m_searchModel.refresh();
	m_searchResultsModel.refresh();
}

void MainWindow::updateSearchResults(const QModelIndex& index) {
	QModelIndex nameCol = m_searchModel.index(index.row(), 0, index.parent());
	QString name = m_searchModel.data(nameCol, Qt::DisplayRole).toString();
	m_searchResultsModel.setVariable(name);
	m_ui->searchName->setText(name);
}

void MainWindow::removeSearch() {
	QModelIndexList indices = m_ui->search->selectionModel()->selectedRows();
	int minRow = INT_MAX;
	int maxRow = 0;
	for (const auto& index : indices) {
		if (m_searchModel.data(index) == m_searchResultsModel.getVariable()) {
			m_searchResultsModel.setVariable(QString());
		}
		if (index.row() < minRow) {
			minRow = index.row();
		}
		if (index.row() > maxRow) {
			maxRow = index.row();
		}
	}

	m_searchModel.removeRows(minRow, maxRow - minRow + 1, QModelIndex());
	m_controller->searchesUpdated();
}

void MainWindow::migrateSearch() {
	QModelIndexList indices = m_ui->searchResults->selectionModel()->selectedRows();
	if (indices.isEmpty()) {
		return;
	}
	Retro::GameData* data = m_searchResultsModel.getDataBacking();
	Retro::Search* search = data->getSearch(m_searchResultsModel.getVariable().toStdString());
	Retro::TypedSearchResult result = search->typedResults()[indices[0].row()];
	data->setVariable(m_searchResultsModel.getVariable().toStdString(), result);
	m_controller->variablesUpdated();
	m_dataModel.refresh();
}

void MainWindow::addVariable() {
	std::string name = m_ui->variableName->text().toStdString();
	std::string type = m_ui->variableType->text().toStdString();
	if (type.size() < 3 || !m_dataModel.getDataBacking()) {
		return;
	}
	m_dataModel.getDataBacking()->setVariable(name, Retro::Variable{ type, static_cast<size_t>(m_ui->variableAddress->value()) });
	m_controller->variablesUpdated();
	m_dataModel.refresh();
}

void MainWindow::removeVariable() {
	QModelIndexList indices = m_ui->variables->selectionModel()->selectedRows();
	int minRow = INT_MAX;
	int maxRow = 0;
	for (const auto& index : indices) {
		if (index.row() < minRow) {
			minRow = index.row();
		}
		if (index.row() > maxRow) {
			maxRow = index.row();
		}
	}

	m_dataModel.removeRows(minRow, maxRow - minRow + 1, QModelIndex());
	m_controller->variablesUpdated();
}

void MainWindow::addCheat() {
	m_cheatModel.addCheat(Cheat{ m_ui->cheatName->text(), m_ui->cheatValue->toPlainText(), true });
	m_ui->cheatName->clear();
	m_ui->cheatValue->clear();
}

void MainWindow::removeCheat() {
	QModelIndexList indices = m_ui->cheats->selectionModel()->selectedRows();
	int minRow = INT_MAX;
	int maxRow = 0;
	for (const auto& index : indices) {
		if (index.row() < minRow) {
			minRow = index.row();
		}
		if (index.row() > maxRow) {
			maxRow = index.row();
		}
	}

	m_cheatModel.removeRows(minRow, maxRow - minRow + 1, QModelIndex());
}

void MainWindow::integrate(const QString& before, const QString& after, const QString& extension) {
	QDir newDir(EmulatorController::dataPath());
	newDir.cd("contrib");
	newDir.mkpath(after);
	newDir.cd(after);
	QString newPath = newDir.filePath("rom." + extension);
	QFile::copy(before, newPath);
	if (m_controller->loadGame(newPath)) {
		QCryptographicHash sha1(QCryptographicHash::Sha1);
		QFile newFile(newPath);
		newFile.open(QIODevice::ReadOnly);
		if (extension == "nes") {
			newFile.read(16);
		}
		sha1.addData(&newFile);

		QByteArray hash = sha1.result().toHex() + '\n';
		QFile hashFile(newDir.filePath("rom.sha"));
		hashFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
		hashFile.write(hash);

		startGame(newDir.absolutePath());
		m_controller->setDataManifest(newDir.filePath("data.json"));
		m_controller->setScenarioManifest(newDir.filePath("scenario.json"));
	}
}

void MainWindow::startGame(const QString& path) {
	m_controller->start();
	changeGame(path);
}

void MainWindow::changeGame(const QString& path) {
	QFileInfo info(path);
	QString mpath = path;
	if (info.isFile() && info.fileName().startsWith("rom.")) {
		info = QFileInfo(info.dir().absolutePath());
		mpath = info.absoluteFilePath();
	}
	m_gameDir = info.isDir() ? mpath : info.dir().absolutePath();
	setWindowFilePath(mpath);
	m_gameName = info.isDir() ? info.completeBaseName() : info.dir().dirName();
	m_stateName.clear();
}

void MainWindow::startEpisode() {
	if (!m_controller || m_gameName.isNull() || m_stateName.isNull()) {
		return;
	}
	m_controller->stopMovie();
	QDir recordDir(m_recordDir);
	QString episodeName = m_gameName + "-" + m_stateName + "-";
	recordDir.setNameFilters({ episodeName + "*.bk2" });
	recordDir.setSorting(QDir::Name);
	QStringList entryList(recordDir.entryList());
	int i = entryList.size();
	episodeName = QString("%1%2").arg(episodeName).arg(i, 4, 10, QLatin1Char('0'));
	m_controller->recordMovie(episodeName + ".bk2", m_gameName);
	m_ui->movie->setText(tr("Recording"));
}
