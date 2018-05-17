#include "DoneSpecWidget.h"
#include "ui_DoneSpecWidget.h"

#include "search.h"

Q_DECLARE_METATYPE(Retro::Operation)

using O = Retro::Operation;

static const QVector<QPair<O, QString>> s_ops{
	{ O::EQUAL, "equal to" },
	{ O::NEGATIVE_EQUAL, "equal to negative" },
	{ O::NOT_EQUAL, "not equal to" },
	{ O::LESS_THAN, "less than" },
	{ O::GREATER_THAN, "greater than" },
	{ O::LESS_OR_EQUAL, "less than or equal to" },
	{ O::GREATER_OR_EQUAL, "greather than or equal to" },
	{ O::NONZERO, "not zero" },
	{ O::ZERO, "zero" },
	{ O::POSITIVE, "positive" },
	{ O::NEGATIVE, "negative" },
};

static const QMap<O, bool> s_usesRef{
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
};

DoneSpecWidget::DoneSpecWidget(QWidget* parent)
	: QWidget(parent)
	, m_ui(new Ui::DoneSpecWidget) {
	m_ui->setupUi(this);
	connect(m_ui->remove, &QAbstractButton::clicked, this, &DoneSpecWidget::removed);
	connect(m_ui->operation, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [this]() {
		O op = m_ui->operation->currentData().value<O>();
		m_ui->reference->setEnabled(s_usesRef.value(op, false));
	});
	for (const auto& op : s_ops) {
		m_ui->operation->addItem(op.second, QVariant::fromValue(op.first));
	}
}

DoneSpecWidget::~DoneSpecWidget() {
	delete m_ui;
}

void DoneSpecWidget::setName(const QString& name) {
	m_name = name;
	m_ui->varName->setText(name);
}

void DoneSpecWidget::setSpec(const Retro::Scenario::DoneSpec& spec) {
	m_ui->delta->setChecked(spec.measurement == Retro::Scenario::Measurement::DELTA);
	m_ui->reference->setValue(spec.reference);
	for (const auto& op : s_ops) {
		if (spec.op == op.first) {
			m_ui->operation->setCurrentIndex(s_ops.indexOf(op));
			break;
		}
	}
}

Retro::Scenario::DoneSpec DoneSpecWidget::getSpec() const {
	Retro::Scenario::DoneSpec spec{
		m_ui->delta->isChecked() ? Retro::Scenario::Measurement::DELTA : Retro::Scenario::Measurement::ABSOLUTE,
		m_ui->operation->currentData().value<O>(),
		m_ui->reference->isEnabled() ? m_ui->reference->value() : 0
	};
	return spec;
}
