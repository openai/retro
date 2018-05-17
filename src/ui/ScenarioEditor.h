#pragma once

#include <QButtonGroup>
#include <QDialog>
#include <QList>
#include <QSet>

#include "data.h"

#include "DoneNodeWidget.h"

namespace Ui {
class ScenarioEditor;
}

class RewardSpecWidget;

class ScenarioEditor : public QDialog {
	Q_OBJECT
public:
	ScenarioEditor(QWidget* parent = nullptr);
	~ScenarioEditor();

	void setScenario(Retro::Scenario*);

	void addRewardVariable(const QString&, const Retro::Scenario::RewardSpec& = { Retro::Scenario::Measurement::DELTA }, unsigned player = 0);
	void addDoneVariable(const QString&, const Retro::Scenario::DoneSpec& = { Retro::Scenario::Measurement::ABSOLUTE });
	void addDoneNode(const QString& name, const Retro::Scenario::DoneNode& = {});

public slots:
	void guessScenario();
	void updateScenario();
	void setActiveDoneNode(DoneNodeWidget*);
	void setRewardPlayer(int);

signals:
	void scenarioUpdated();
	void scenarioSaved(const QString&);

private:
	void updateDoneNode(DoneNodeWidget*, const QString& name = {});
	void populateScriptLists();
	void resetRewardFunc();

	Ui::ScenarioEditor* m_ui;
	Retro::Scenario* m_scenario;

	struct {
		QList<RewardSpecWidget*> widgets;
		QWidget* page;
		QSet<QString> vars;
		QString func;
	} m_reward[Retro::MAX_PLAYERS]{};

	DoneNodeWidget m_rootDoneNode;
	DoneNodeWidget* m_activeDoneNode = &m_rootDoneNode;
	QString m_activeDoneNodeName;

	unsigned m_activePlayer = 0;

	QButtonGroup m_nodes;
};
