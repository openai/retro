#pragma once

#include <QMap>
#include <QRadioButton>
#include <QSet>
#include <QWidget>

#include "DoneSpecWidget.h"

#include "data.h"

namespace Ui {
class DoneNodeWidget;
}

class DoneNodeWidget : public QWidget {
	Q_OBJECT
public:
	DoneNodeWidget(QWidget* parent = nullptr);
	~DoneNodeWidget();

	QList<DoneSpecWidget*> variableWidgets() const;
	QMap<QString, DoneNodeWidget*> nodeWidgets() const;

	Retro::Scenario::DoneCondition condition() const;

	DoneNodeWidget* findNode(const QString& name);

	void setNode(const Retro::Scenario::DoneNode&);
	std::shared_ptr<Retro::Scenario::DoneNode> makeNode() const;

	QRadioButton* getActiveButton();

	void addVariable(const QString&, const Retro::Scenario::DoneSpec& = {});
	void addNode(const QString& name, const Retro::Scenario::DoneNode& = {});

public slots:
	void setCondition(Retro::Scenario::DoneCondition);
	void clear();

signals:
	void removed();

private:
	Ui::DoneNodeWidget* m_ui;

	QList<DoneSpecWidget*> m_widgets;
	QSet<QString> m_vars;
	QMap<QString, DoneNodeWidget*> m_nodes;
};
