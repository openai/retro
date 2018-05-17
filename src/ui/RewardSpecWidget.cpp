#include "RewardSpecWidget.h"
#include "ui_RewardSpecWidget.h"

#include "search.h"

Q_DECLARE_METATYPE(Retro::Operation)

using O = Retro::Operation;

static const QVector<QPair<O, QString>> s_ops{
	{ O::NOOP, "per" },
	{ O::EQUAL, "if equal to" },
	{ O::NEGATIVE_EQUAL, "if equal to negative" },
	{ O::NOT_EQUAL, "if not equal to" },
	{ O::LESS_THAN, "if less than" },
	{ O::GREATER_THAN, "if greater than" },
	{ O::LESS_OR_EQUAL, "if less than or equal to" },
	{ O::GREATER_OR_EQUAL, "if greather than or equal to" },
	{ O::NONZERO, "if not zero" },
	{ O::ZERO, "if zero" },
	{ O::POSITIVE, "if positive" },
	{ O::NEGATIVE, "if negative" },
	{ O::SIGN, "based on sign" },
};

static const QMap<O, bool> s_usesRef{
	{ O::NOOP, false },
	{ O::EQUAL, true },
	{ O::NEGATIVE_EQUAL, true },
	{ O::NOT_EQUAL, true },
	{ O::LESS_THAN, true },
	{ O::GREATER_THAN, true },
	{ O::LESS_OR_EQUAL, true },
	{ O::GREATER_OR_EQUAL, true },
	{ O::NONZERO, false },
	{ O::ZERO, false },
	{ O::POSITIVE, false },
	{ O::NEGATIVE, false },
	{ O::SIGN, false },
};

RewardSpecWidget::RewardSpecWidget(QWidget* parent)
	: QWidget(parent)
	, m_ui(new Ui::RewardSpecWidget) {
	m_ui->setupUi(this);
	connect(m_ui->remove, &QAbstractButton::clicked, this, &RewardSpecWidget::removed);
	connect(m_ui->operation, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [this]() {
		O op = m_ui->operation->currentData().value<O>();
		m_ui->reference->setEnabled(s_usesRef.value(op, false));
	});
	for (const auto& op : s_ops) {
		m_ui->operation->addItem(op.second, QVariant::fromValue(op.first));
	}
}

RewardSpecWidget::~RewardSpecWidget() {
	delete m_ui;
}

void RewardSpecWidget::setName(const QString& name) {
	m_name = name;
	m_ui->varName->setText(name);
}

void RewardSpecWidget::setSpec(const Retro::Scenario::RewardSpec& spec) {
	m_ui->delta->setChecked(spec.measurement == Retro::Scenario::Measurement::DELTA);
	m_ui->reward->setValue(spec.reward);
	m_ui->penalty->setValue(spec.penalty);
	m_ui->reference->setValue(spec.reference);
	for (const auto& op : s_ops) {
		if (spec.op == op.first) {
			m_ui->operation->setCurrentIndex(s_ops.indexOf(op));
			break;
		}
	}
}

Retro::Scenario::RewardSpec RewardSpecWidget::getSpec() const {
	Retro::Scenario::RewardSpec spec{
		m_ui->delta->isChecked() ? Retro::Scenario::Measurement::DELTA : Retro::Scenario::Measurement::ABSOLUTE,
		m_ui->operation->currentData().value<O>(),
		m_ui->reference->isEnabled() ? m_ui->reference->value() : 0,
		static_cast<float>(m_ui->reward->value()),
		static_cast<float>(m_ui->penalty->value())
	};
	return spec;
}
