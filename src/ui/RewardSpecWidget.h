#pragma once

#include <QWidget>

#include "data.h"

namespace Ui {
class RewardSpecWidget;
}

class RewardSpecWidget : public QWidget {
	Q_OBJECT
public:
	RewardSpecWidget(QWidget* parent = nullptr);
	~RewardSpecWidget();

	void setName(const QString&);
	QString getName() const { return m_name; }

	void setSpec(const Retro::Scenario::RewardSpec&);
	Retro::Scenario::RewardSpec getSpec() const;

signals:
	void removed();

private:
	Ui::RewardSpecWidget* m_ui;
	QString m_name;
};
