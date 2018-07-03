#include "DoneNodeWidget.h"
#include "ui_DoneNodeWidget.h"

#include "DoneSpecWidget.h"

DoneNodeWidget::DoneNodeWidget(QWidget* parent)
	: QWidget(parent)
	, m_ui(new Ui::DoneNodeWidget) {
	m_ui->setupUi(this);
	connect(m_ui->remove, &QAbstractButton::clicked, this, &DoneNodeWidget::removed);
}

DoneNodeWidget::~DoneNodeWidget() {
	delete m_ui;
}

QList<DoneSpecWidget*> DoneNodeWidget::variableWidgets() const {
	return m_widgets;
}

QMap<QString, DoneNodeWidget*> DoneNodeWidget::nodeWidgets() const {
	return m_nodes;
}

Retro::Scenario::DoneCondition DoneNodeWidget::condition() const {
	return m_ui->conditionAll->isChecked() ? Retro::Scenario::DoneCondition::ALL : Retro::Scenario::DoneCondition::ANY;
}

DoneNodeWidget* DoneNodeWidget::findNode(const QString& name) {
	if (name.isEmpty()) {
		return this;
	}
	QString section = name.section('.', 0, 0);
	DoneNodeWidget* parent = m_nodes[section];
	if (!parent) {
		return nullptr;
	}
	return parent->findNode(name.section('.', 1));
}

void DoneNodeWidget::setNode(const Retro::Scenario::DoneNode& node) {
	m_nodes.clear();
	m_vars.clear();

	for (const auto& var : node.vars) {
		addVariable(QString::fromStdString(var.first), var.second);
	}
	for (const auto& subnode : node.nodes) {
		addNode(QString::fromStdString(subnode.first), *subnode.second);
	}

	setCondition(node.condition);
}

std::shared_ptr<Retro::Scenario::DoneNode> DoneNodeWidget::makeNode() const {
	auto node = std::make_shared<Retro::Scenario::DoneNode>();
	node->condition = condition();

	for (const auto& var : m_widgets) {
		node->vars.emplace(var->getName().toStdString(), var->getSpec());
	}

	for (const auto& name : m_nodes.keys()) {
		node->nodes.emplace(name.toStdString(), m_nodes[name]->makeNode());
	}

	return node;
}

QRadioButton* DoneNodeWidget::getActiveButton() {
	return m_ui->active;
}

void DoneNodeWidget::setCondition(Retro::Scenario::DoneCondition condition) {
	if (condition == Retro::Scenario::DoneCondition::ALL) {
		m_ui->conditionAll->setChecked(true);
	} else {
		m_ui->conditionAny->setChecked(true);
	}
}

void DoneNodeWidget::addVariable(const QString& name, const Retro::Scenario::DoneSpec& spec) {
	if (m_vars.contains(name)) {
		return;
	}
	DoneSpecWidget* widget = new DoneSpecWidget;
	widget->setName(name);
	widget->setSpec(spec);
	m_ui->nodeList->addWidget(widget);
	m_widgets.append(widget);
	m_vars.insert(name);
	connect(widget, &DoneSpecWidget::removed, [this, widget]() {
		m_ui->nodeList->removeWidget(widget);
		m_widgets.removeAll(widget);
		m_vars.remove(widget->getName());
		widget->deleteLater();
	});
}

void DoneNodeWidget::addNode(const QString& name, const Retro::Scenario::DoneNode& node) {
	if (name.contains('.')) {
		QString section = name.section('.', 0, 0);
		if (!m_nodes.contains(section)) {
			return;
		}
		DoneNodeWidget* parent = m_nodes[section];
		parent->addNode(name.section('.', 1), node);
		return;
	}
	if (m_nodes.contains(name)) {
		return;
	}
	DoneNodeWidget* widget = new DoneNodeWidget;
	widget->setNode(node);
	m_ui->nodeList->addWidget(widget);
	m_nodes[name] = widget;
	connect(widget, &DoneNodeWidget::removed, [this, name, widget]() {
		m_ui->nodeList->removeWidget(widget);
		for (auto& child : widget->m_nodes) {
			emit child->removed();
		}
		for (auto& child : widget->m_widgets) {
			emit child->removed();
		}
		m_nodes.remove(name);
		widget->deleteLater();
	});
}

void DoneNodeWidget::clear() {
	for (auto& child : m_nodes) {
		emit child->removed();
	}
	for (auto& child : m_widgets) {
		emit child->removed();
	}
}
