#include "ScenarioEditor.h"
#include "ui_ScenarioEditor.h"

#include <QFileDialog>
#include <QVBoxLayout>

#include "script.h"

#include "DoneSpecWidget.h"
#include "RewardSpecWidget.h"

ScenarioEditor::ScenarioEditor(QWidget* parent)
	: QDialog(parent)
	, m_ui(new Ui::ScenarioEditor) {
	m_ui->setupUi(this);

	m_ui->doneList->layout()->addWidget(&m_rootDoneNode);
	m_nodes.addButton(m_rootDoneNode.getActiveButton());
	m_rootDoneNode.getActiveButton()->setChecked(true);
	updateDoneNode(&m_rootDoneNode);

	connect(m_ui->addReward, &QAbstractButton::pressed, [this]() {
		addRewardVariable(m_ui->variableName->currentText(), { Retro::Scenario::Measurement::DELTA }, m_activePlayer);
	});

	connect(m_ui->addDone, &QAbstractButton::pressed, [this]() {
		addDoneVariable(m_ui->variableName->currentText());
	});

	connect(m_ui->addDoneNode, &QAbstractButton::pressed, [this]() {
		addDoneNode(m_ui->doneNodeName->text(), {});
		setActiveDoneNode(m_activeDoneNode->findNode(m_ui->doneNodeName->text()));
	});

	connect(m_ui->guess, &QAbstractButton::pressed, this, &ScenarioEditor::guessScenario);

	connect(m_ui->loadScript, &QAbstractButton::pressed, [this]() {
		QString script = QFileDialog::getOpenFileName(this, tr("Select script"), QString(), tr("Script file (*.lua)"));
		if (!script.isNull()) {
			int dot = script.lastIndexOf(QChar('.'));
			QString extName;
			if (dot > 0) {
				extName = script.right(script.length() - dot - 1);
			}
			if (m_scenario->loadScript(script.toStdString(), extName.toStdString())) {
				populateScriptLists();
			}
		}
	});

	connect(m_ui->reloadScripts, &QAbstractButton::pressed, [this]() {
		m_scenario->reloadScripts();
		populateScriptLists();
	});

	connect(this, &QDialog::accepted, this, &ScenarioEditor::updateScenario);
	connect(m_ui->buttonBox->button(QDialogButtonBox::Save), &QAbstractButton::pressed, [this]() {
		updateScenario();
		QString manifest = QFileDialog::getSaveFileName(this, tr("Select scenario.json"), QString(), tr("JSON file (*.json)"));
		if (!manifest.isNull()) {
			m_scenario->save(manifest.toStdString());
			emit scenarioSaved(manifest);
		}
	});

	m_ui->playerId->setMaximum(Retro::MAX_PLAYERS);
	connect(m_ui->playerId, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &ScenarioEditor::setRewardPlayer);
	for (unsigned p = 0; p < Retro::MAX_PLAYERS; ++p) {
		m_reward[p].page = new QWidget;
		m_reward[p].page->setLayout(new QVBoxLayout);
		m_ui->rewardList->addWidget(m_reward[p].page);
	}
}

ScenarioEditor::~ScenarioEditor() {
	delete m_ui;
}

void ScenarioEditor::setScenario(Retro::Scenario* scenario) {
	for (unsigned p = 0; p < Retro::MAX_PLAYERS; ++p) {
		for (auto& child : m_reward[p].widgets) {
			emit child->removed();
		}
	}
	m_rootDoneNode.clear();

	populateScriptLists();
	for (const auto& done : scenario->listDoneVariables()) {
		m_rootDoneNode.addVariable(QString::fromStdString(done.first), done.second);
	}
	for (const auto& node : scenario->listDoneNodes()) {
		addDoneNode(QString::fromStdString(node.first), *node.second);
	}

	m_rootDoneNode.setCondition(scenario->doneCondition());

	while (m_ui->variableName->count()) {
		m_ui->variableName->removeItem(0);
	}
	for (const auto& var : scenario->data()->listVariables()) {
		m_ui->variableName->addItem(QString::fromStdString(var.first));
	}

	for (unsigned p = 0; p < Retro::MAX_PLAYERS; ++p) {
		for (const auto& reward : scenario->listRewardVariables(p)) {
			addRewardVariable(QString::fromStdString(reward.first), reward.second, p);
		}
		std::pair<std::string, std::string> func = scenario->rewardFunction(p);
		if (!func.first.empty()) {
			m_reward[p].func = QString("%1:%2")
								   .arg(QString::fromStdString(func.second))
								   .arg(QString::fromStdString(func.first));
		} else {
			m_reward[p].func = QString();
		}
	}
	resetRewardFunc();
	const auto& func = scenario->doneFunction();
	if (!func.first.empty()) {
		QString funcContext = QString("%1:%2")
								  .arg(QString::fromStdString(func.second))
								  .arg(QString::fromStdString(func.first));
		m_ui->doneFuncUse->setChecked(true);
		m_ui->doneFunc->setEnabled(true);
		int item = m_ui->doneFunc->findText(funcContext);
		if (item > -1) {
			m_ui->doneFunc->setCurrentIndex(item);
		}
	}
	m_ui->rewardList->setCurrentIndex(0);
	m_scenario = scenario;
}

void ScenarioEditor::addRewardVariable(const QString& name, const Retro::Scenario::RewardSpec& spec, unsigned player) {
	if (m_reward[player].vars.contains(name)) {
		return;
	}
	RewardSpecWidget* widget = new RewardSpecWidget;
	widget->setName(name);
	widget->setSpec(spec);
	m_reward[player].page->layout()->addWidget(widget);
	m_reward[player].widgets.append(widget);
	m_reward[player].vars.insert(name);
	connect(widget, &RewardSpecWidget::removed, [this, widget, name, player]() {
		m_reward[player].page->layout()->removeWidget(widget);
		m_reward[player].widgets.removeAll(widget);
		m_reward[player].vars.remove(name);
		widget->deleteLater();
	});
}

void ScenarioEditor::addDoneVariable(const QString& name, const Retro::Scenario::DoneSpec& spec) {
	m_activeDoneNode->addVariable(name, spec);
}

void ScenarioEditor::addDoneNode(const QString& name, const Retro::Scenario::DoneNode& node) {
	if (name.isEmpty()) {
		return;
	}
	m_activeDoneNode->addNode(name, node);
	DoneNodeWidget* widget = m_activeDoneNode->findNode(name);
	updateDoneNode(widget, name.section('.', -1));
}

void ScenarioEditor::guessScenario() {
	using M = Retro::Scenario::Measurement;
	using O = Retro::Operation;
	auto variables = m_scenario->data()->listVariables();
	if (variables.find("lives") != variables.end()) {
		Retro::Variable lives = variables.at("lives");
		if (lives.type.repr == Retro::Repr::SIGNED) {
			addDoneVariable("lives", { M::ABSOLUTE, O::NEGATIVE });
		} else {
			addDoneVariable("lives", { M::ABSOLUTE, O::ZERO });
		}
	} else if (variables.find("health") != variables.end()) {
		Retro::Variable lives = variables.at("health");
		if (lives.type.repr == Retro::Repr::SIGNED) {
			addDoneVariable("health", { M::ABSOLUTE, O::NEGATIVE });
		} else {
			addDoneVariable("health", { M::ABSOLUTE, O::ZERO });
		}
	}
	if (variables.find("score") != variables.end()) {
		addRewardVariable("score", { M::DELTA, O::NOOP, 0, 1 });
	}
}

void ScenarioEditor::updateScenario() {
	if (!m_scenario) {
		return;
	}
	m_scenario->reset();

	for (unsigned p = 0; p < Retro::MAX_PLAYERS; ++p) {
		for (const auto& widget : m_reward[p].widgets) {
			m_scenario->setRewardVariable(widget->getName().toStdString(), widget->getSpec(), p);
		}

		if (!m_reward[p].func.isEmpty()) {
			QStringList func = m_reward[p].func.split(QChar(':'));
			m_scenario->setRewardFunction(func[1].toStdString(), func[0].toStdString(), p);
		} else {
			m_scenario->setRewardFunction({}, {}, p);
		}
	}

	for (const auto& widget : m_rootDoneNode.variableWidgets()) {
		m_scenario->setDoneVariable(widget->getName().toStdString(), widget->getSpec());
	}

	const auto& nodes = m_rootDoneNode.nodeWidgets();
	for (const auto& name : nodes.keys()) {
		m_scenario->setDoneNode(name.toStdString(), nodes[name]->makeNode());
	}

	if (m_rootDoneNode.variableWidgets().size() + m_rootDoneNode.nodeWidgets().size() < 2) {
		m_scenario->setDoneCondition(Retro::Scenario::DoneCondition::ANY);
	} else {
		m_scenario->setDoneCondition(m_rootDoneNode.condition());
	}

	if (m_ui->doneFuncUse->isChecked()) {
		QStringList func = m_ui->doneFunc->currentText().split(QChar(':'));
		m_scenario->setDoneFunction(func[1].toStdString(), func[0].toStdString());
	} else {
		m_scenario->setDoneFunction({}, {});
	}
	emit scenarioUpdated();
}

void ScenarioEditor::setActiveDoneNode(DoneNodeWidget* node) {
	m_activeDoneNode = node;
	QRadioButton* button = node->getActiveButton();
	button->setChecked(true);
}

void ScenarioEditor::setRewardPlayer(int player) {
	if (player < 1 || player > Retro::MAX_PLAYERS) {
		return;
	}
	m_activePlayer = player - 1;
	m_ui->rewardList->setCurrentIndex(m_activePlayer);
	resetRewardFunc();
}

void ScenarioEditor::updateDoneNode(DoneNodeWidget* node, const QString& name) {
	QRadioButton* button = node->getActiveButton();
	button->setText(name);
	m_nodes.addButton(button);

	connect(button, &QAbstractButton::clicked, [this, node]() {
		setActiveDoneNode(node);
	});
	connect(node, &DoneNodeWidget::removed, [this, node]() {
		if (node == m_activeDoneNode && node != &m_rootDoneNode) {
			setActiveDoneNode(&m_rootDoneNode);
		}
	});

	auto subnodes = node->nodeWidgets();
	for (auto name : subnodes.keys()) {
		updateDoneNode(subnodes[name], name);
	}
}

void ScenarioEditor::populateScriptLists() {
	m_ui->rewardFuncUse->setChecked(false);
	m_ui->rewardFuncUse->setEnabled(false);
	m_ui->rewardFunc->clear();
	m_ui->rewardFunc->setEnabled(false);

	m_ui->doneFuncUse->setChecked(false);
	m_ui->doneFuncUse->setEnabled(false);
	m_ui->doneFunc->clear();
	m_ui->doneFunc->setEnabled(false);

	for (const auto& contextName : Retro::ScriptContext::listContexts()) {
		std::shared_ptr<Retro::ScriptContext> context = Retro::ScriptContext::get(contextName);
		const auto& funcs = context->listFunctions();
		if (!funcs.empty()) {
			m_ui->rewardFuncUse->setEnabled(true);
			m_ui->doneFuncUse->setEnabled(true);
		}
		for (const auto& func : funcs) {
			QString funcContext = QString("%1:%2")
									  .arg(QString::fromStdString(contextName))
									  .arg(QString::fromStdString(func));

			m_ui->rewardFunc->addItem(funcContext);
			m_ui->doneFunc->addItem(funcContext);
		}
	}
}

void ScenarioEditor::resetRewardFunc() {
	QString rewardFunc = m_reward[m_activePlayer].func;
	if (rewardFunc.isEmpty()) {
		m_ui->rewardFuncUse->setChecked(false);
	} else {
		m_ui->rewardFuncUse->setChecked(true);
		int item = m_ui->rewardFunc->findText(rewardFunc);
		if (item > -1) {
			m_ui->rewardFunc->setCurrentIndex(item);
		}
	}
}
