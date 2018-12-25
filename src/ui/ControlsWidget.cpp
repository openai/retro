#include "ControlsWidget.h"
#include "ui_ControlsWidget.h"

#include <QGroupBox>
#include <QKeySequenceEdit>
#include <QLabel>

ControlsWidget::ControlsWidget(const QString& platform, QWidget* parent)
	: QDialog(parent)
	, m_ui(new Ui::ControlsWidget) {
	m_ui->setupUi(this);
	m_ui->platform->setTitle(platform);
	m_sections.append(m_ui->platform);
}

ControlsWidget::~ControlsWidget() {
	delete m_ui;
}

void ControlsWidget::setKeys(const QStringList& keys) {
	m_keyNames = keys;
	for (unsigned player = 0; player < m_bindings.size(); ++player) {
		for (const auto& key : m_bindings[player].keys()) {
			if (!m_keyNames.contains(key)) {
				m_bindings[player].remove(key);
			}
		}
	}
}

void ControlsWidget::setBindings(const QMap<QString, int>& bindings, unsigned player) {
	m_bindings[player].clear();
	while (m_sections[player]->layout()->count()) {
		delete m_sections[player]->layout()->takeAt(0);
	}
	for (const auto& key : m_keyNames) {
		if (bindings.contains(key)) {
			m_bindings[player][key] = bindings[key];
			QKeySequenceEdit* edit = new QKeySequenceEdit(QKeySequence(bindings[key]));
			QFormLayout* layout = static_cast<QFormLayout*>(m_sections[player]->layout());
			layout->addRow(key, edit);
			connect(edit, &QKeySequenceEdit::keySequenceChanged, [this, key, player](const QKeySequence& seq) {
				m_bindings[player][key] = seq[0];
			});
		}
	}
}

void ControlsWidget::setPlayers(unsigned players) {
	m_players = players;
	while (m_bindings.size() < players) {
		m_bindings.append(QMap<QString, int>{});
	}
	while (m_bindings.size() > players) {
		m_bindings.removeLast();
	}
	while (m_sections.size() < players) {
		QGroupBox* box = new QGroupBox;
		m_sections.append(box);
		box->setTitle(tr("Player %1").arg(m_sections.size()));
		box->setLayout(new QFormLayout);
		m_ui->players->addWidget(box);
	}
	while (m_sections.size() > players) {
		delete m_sections.takeLast();
	}
}

QMap<QString, int> ControlsWidget::getBindings(unsigned player) const {
	return m_bindings[player];
}

void ControlsWidget::accept() {
	for (unsigned player = 0; player < m_players; ++player) {
		for (const auto& key : m_keyNames) {
			if (m_bindings[player].contains(key)) {
				emit bindingChanged(key, m_bindings[player][key], player);
			}
		}
	}
	close();
}
